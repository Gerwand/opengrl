#include <grl/rdf/RDFUtils.h>
#include <grl/rdf/RandomDecisionForest.h>

namespace grl {

void
RandomDecisionForest::train(const ForestTrainContext &context)
{
#ifdef USE_GPU
    if (context.gpuContext != nullptr) {
        std::ifstream file("rdf.cl");
        std::string kernels((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        cl::Program::Sources sources;
        sources.push_back({kernels.c_str(), kernels.length()});

        context.gpuContext->program = cl::Program(context.gpuContext->context, sources);
        if (context.gpuContext->program.build({context.gpuContext->device}) != CL_SUCCESS) {
            std::cout << "Failed to build sources: "
                << context.gpuContext->program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.gpuContext->device)
                << "\n";
            exit(EINVAL);
        }
    }
#endif

    for (size_t i = 0; i < _trees.size(); i += context.nthreads) {
        size_t size = std::min(_trees.size() - i, context.nthreads);
        for (size_t n = 0; n < size; ++n)
            _threads.push_back(std::thread(&RandomDecisionForest::trainTree, &_trees[i + n], &context));

        for (auto it = _threads.begin(); it != _threads.cend(); ++it)
            it->join();
        _threads.clear();
    }
}

void
RandomDecisionForest::saveToFile(const std::string &fileName)
{
    std::ofstream file;
    file.open(fileName, std::ofstream::out);

    int i = 0;
    for (auto it = _trees.begin(); it != _trees.end(); ++it, ++i) {
        file << "T" << i << "\n";
        it->saveToFile(file);
    }
}

bool
RandomDecisionForest::loadFromFile(const std::string &fileName)
{
    std::ifstream file;
    file.open(fileName, std::ifstream::in);
    if (!file.is_open())
        return false;

    int treeNum = 0;
    while (!file.eof()) {
        // Check if the format is valid
        char cmd;
        file >> cmd;
        if (cmd != 'T')
            return file.eof();

        int num;
        file >> num;
        if (num != treeNum)
            return file.eof();

        _trees.push_back(DecisionTree());
        _trees.back().readFromFile(file);

        ++treeNum;
    }

    return true;
}

using ProbabilitiesVector = std::vector<const std::vector<float> *>;

void
RandomDecisionForest::classifyImage(const cv::Mat &depthImage, cv::Mat &classImage,
                                    ClassesWeights &weights, ClassesPoints &bestPoints)
{
    int width = depthImage.cols;
    int height = depthImage.rows;

    classImage = cv::Mat(height, width, CV_8SC1, cv::Scalar(grlBackgroundIndex));

    // Initialize weights map for each class with 0
    for (auto it = weights.begin(); it != weights.end(); ++it)
        *it = cv::Mat::zeros(cv::Size(width, height), CV_32FC1);

    // Initialize array with list of the best points
    for (auto it = bestPoints.begin(); it != bestPoints.end(); ++it, ++it)
        it->clear();

    auto itd = depthImage.begin<float>();
    for (short y = 0; y < height; ++y) {
        for (short x = 0; x < width; ++x, ++itd) {
            // Check if the pixel is background. It is background if it's value
            // is very close to 0 or higher than max allowed distance.
            if (*itd > grlDepthMaxDist || *itd < grl::epsilon)
                continue;

            // Create abstraction of the pixel
            Pixel p = { { x, y },   // Coordinates in the image
                         *itd,      // Depth value of the pixel
                         0,         // Irrelevant, only needed for learning
                         grlUnknownIndex }; // Default value, will be overriden

            // Calculate probabilities and get the best fitting class
            std::vector<float> probabilities;
            std::pair<float, int8_t> bestClassScore = getClassForPixel(
                depthImage,
                p,
                probabilities
            );
            classImage.at<char>(cv::Point(x, y)) = bestClassScore.second;

            // Copy the probability vector inside the image holding the weights.
            // The weight is calculated as: P(c)*d(x)^2 for being depth invariant
            auto its = probabilities.begin();
            for (auto itw = weights.begin(); itw != weights.end(); ++itw, ++its)
                itw->at<float>(cv::Point(x, y)) = (*its) * p.depth*p.depth;

            // Update best points list. Points with the best probability in their
            // class will be considered as the starting search point for the
            // joint. Mutlimap is sorting its element by its keys, so it takes
            // the sorting problem on its own.
            std::multimap<float, Vec2i> &classBestPoints = bestPoints[bestClassScore.second];
            if (classBestPoints.size() < grlBestPointsNum) {
                classBestPoints.insert(std::make_pair(bestClassScore.first, Vec2i(x, y)));
            } else {
                auto smallestScoreElement = classBestPoints.begin();
                if (bestClassScore.first > smallestScoreElement->first) {
                    classBestPoints.erase(smallestScoreElement);
                    classBestPoints.insert(std::make_pair(bestClassScore.first, Vec2i(x, y)));
                }
            }
        }
    }
}

std::pair<float, int8_t> RandomDecisionForest::getClassForPixel(
    const cv::Mat &depthImage,
    const Pixel &pixel,
    std::vector<float> &probabilitiesSum)
{
    probabilitiesSum.clear();

    // Let all of the trees vote. We will receive probabilites of the
    // classes for each pixel.
    ProbabilitiesVector probabilitiesVotes;
    for (auto itTree = _trees.begin(); itTree != _trees.cend(); ++itTree)
        probabilitiesVotes.push_back(&itTree->classifyPixel(depthImage, pixel));

    // Let's calculate average probability taking into consideration
    // results from each tree.
    size_t n = 0;
    for (size_t n = 0; n < grlHandIndexNum; ++n) {
        float sum = 0;
        for (auto itVote = probabilitiesVotes.cbegin(); itVote != probabilitiesVotes.cend(); ++itVote)
            sum += (*itVote)->at(n);
        probabilitiesSum.push_back(sum / _trees.size());
    }

    // Get maximum probability - it will be assigned to the pixels as
    // its class.
    std::pair<float, int8_t> maxProb(-std::numeric_limits<float>::infinity(), 0);
    int8_t i = 0;
    for (auto itSum = probabilitiesSum.cbegin(); itSum != probabilitiesSum.cend(); ++itSum, ++i) {
        if (maxProb.first < *itSum)
            maxProb = std::make_pair(*itSum, i);
    }

    return maxProb;
}

void
RandomDecisionForest::trainTree(DecisionTree *tree, const ForestTrainContext *context)
{
    static std::hash<std::thread::id> hasher;

#ifdef USE_GPU
    TreeTrainGPUContext treeGPU;
    bool useGPU = false;
    size_t pixelsNum = sizeof(Pixel) * context->pixelsPerImage * context->classImages.size();

    cl_int err = 0;
    if (context->gpuContext != nullptr) {
        useGPU = true;
        treeGPU.device = context->gpuContext->device;
        treeGPU.context = context->gpuContext->context;
        treeGPU.program = context->gpuContext->program;
        treeGPU.queue = cl::CommandQueue(treeGPU.context, treeGPU.device);
        treeGPU.maxImages = context->gpuContext->maxImages;

        treeGPU.bufferAllPix = cl::Buffer(context->gpuContext->context, CL_MEM_READ_ONLY,
                                          sizeof(Pixel)*pixelsNum, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Pix buffer error" <<  std::endl;
            exit(-1);
        }
        treeGPU.bufferSplit = cl::Buffer(context->gpuContext->context, CL_MEM_WRITE_ONLY,
                                         sizeof(cl_char)*pixelsNum, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Split buffer error" <<  std::endl;
            exit(-1);
        }
        treeGPU.bufferPix = cl::Buffer(context->gpuContext->context, CL_MEM_WRITE_ONLY,
                                       sizeof(Pixel)*pixelsNum, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Pix2 buffer error" <<  std::endl;
            exit(-1);
        }
        treeGPU.bufferPixCount = cl::Buffer(context->gpuContext->context, CL_MEM_WRITE_ONLY,
                                            sizeof(cl_uint)*grlHandIndexNum, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Pixcount buffer error" <<  std::endl;
            exit(-1);
        }

        treeGPU.getFeatureTrain = cl::Kernel(treeGPU.program, "getFeatureTrain", &err);
        treeGPU.getProbabilities = cl::Kernel(treeGPU.program, "getProbabilities", &err);
    }
#endif
    // Chose class pixels from each class image
    std::vector<Pixel> *pixels = new std::vector<Pixel>;
    pixels->reserve(context->pixelsPerImage * context->classImages.size());

    std::mt19937 gen(static_cast<unsigned int>(clock() + hasher(std::this_thread::get_id())));
    printf("Generating...\n");
    uint32_t imgID = 0;
    for (auto itc = context->classImages.cbegin(), itd = context->depthImages.cbegin();
         itc != context->classImages.cend(); ++itc, ++itd, ++imgID) {
        std::uniform_int_distribution<> coordXRand(0, itc->cols - 1);
        std::uniform_int_distribution<> coordYRand(0, itc->rows - 1);
        cv::Mat imgMask = cv::Mat::zeros(itc->rows, itc->cols, CV_8SC1);

        size_t pixelsAcquired = 0;
        int tryAgain = 0;
        while (pixelsAcquired < context->pixelsPerImage) {
            cv::Point coords(coordXRand(gen), coordYRand(gen));

            // Check if this pixel was analyzed
            int8_t &maskVal = imgMask.at<int8_t>(coords);
            if (maskVal == 0) {
                maskVal = 1;
                tryAgain = 0;
            } else {
                // If we are finding masked pixel for 1000 times contiously, break the loop
                if (++tryAgain == 1000)
                    break;
                continue;
            }

            // If the pixel is background, try again
            int8_t index = itc->at<int8_t>(coords);
            if (index == grlBackgroundIndex)
                continue;

            pixels->push_back(Pixel{
#ifdef USE_GPU
                Vector2<cl_short>{static_cast<cl_short>(coords.x), static_cast<cl_short>(coords.y)}, // Coords
#else
                Vector2<short>{static_cast<short>(coords.x), static_cast<short>(coords.y)}, // Coords
#endif
                itd->at<float>(coords), // Depth
                imgID,
                index});
            ++pixelsAcquired;
        }
    }

    printf("Tree training...\n");
    tree->train(pixels, context->depthImages, context->nodeTrainLimit, context->maxDepth, gen,
#ifdef USE_GPU
                useGPU ? &treeGPU : nullptr);
#else
                nullptr);
#endif
}

}