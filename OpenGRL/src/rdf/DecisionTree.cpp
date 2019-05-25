#include <grl/rdf/DecisionTree.h>

#include <cassert>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace grl {

void
Node::setLeaf(bool leaf)
{
    _isLeaf = leaf;
    if (_isLeaf) {
        if (_left.get() != nullptr) {
            delete _left->getPixels();
            _left->setPixels(nullptr);
        }
        if (_right.get() != nullptr) {
            delete _right->getPixels();
            _right->setPixels(nullptr);
        }
        _left.release();
        _right.release();
    }
}

uint8_t
Node::evaluateFeature(const cv::Mat &depthImage, const Pixel &p) const
{
    assert(!_isLeaf);
    bool isInf;

    // Offset u
    Vec2i tu = Vec2i{p.coords.x + static_cast<int>(std::round(_decision.u.x/p.depth)),
        p.coords.y + static_cast<int>(std::round(_decision.u.y/p.depth))};
    // Check if on border
    isInf = !isBetween(tu.x, depthImage.cols-1, 0) || !isBetween(tu.y, depthImage.rows-1, 0);
    if (isInf) {
        return grlNodeGoRight;
    }
    // Check is depth is background
    float udepth = depthImage.at<float>(tu.y, tu.x);
    isInf = udepth >= 8.0f;
    if (isInf) {
        return grlNodeGoRight;
    }

    // Offset v
    Vec2i tv = Vec2i{p.coords.x + static_cast<int>(std::round(_decision.v.x/p.depth)),
        p.coords.y + static_cast<int>(std::round(_decision.v.y/p.depth))};
    // Check if on border
    isInf = !isBetween(tv.x, depthImage.cols-1, 0) || !isBetween(tv.y, depthImage.rows-1, 0);
    if (isInf) {
        return grlNodeGoRight;
    }
    // Check is depth is background
    float vdepth = depthImage.at<float>(tv.y, tv.x);
    isInf = vdepth >= 8.0f;
    if (isInf) {
        return grlNodeGoRight;
    }

    if ((udepth - vdepth) < _decision.t) {
        return grlNodeGoLeft;
    } else {
        return grlNodeGoRight;
    }

    // Assuming that pixel is not the background pixel
    //return ? grlNodeGoLeft : grlNodeGoRight;
}

void
Node::saveToFile(std::ofstream &file)
{
    assert(file.is_open());
    // Go left
    if (!_isLeaf) {
        file << ".L\n";
        _left->saveToFile(file);

        // Go right
        file << ".R\n";
        _right->saveToFile(file);

        // Decision node
        file << ">D\n";
        file << _decision.u.x << '\n' << _decision.u.y << '\n'
            << _decision.v.x << '\n' << _decision.v.y << '\n'
            << _decision.t << '\n';
    } else {
        if (_isLeaf) {
            // Leaf node
            file << ">L\n";
            file << '#' << _probabilities.size() << '\n';
            for (auto it = _probabilities.cbegin(); it != _probabilities.cend(); ++it)
                file << *it << '\n';
            // Go up

        }
    }
    file << ".U\n";
}

const Node *
Node::getLeafForPixel(const cv::Mat &depthImage, const Pixel &p)
{
    const Node *node = this;

    while (!node->isLeaf()) {
        uint8_t way = node->evaluateFeature(depthImage, p);
        node = way == grlNodeGoLeft ? node->getLeft() : node->getRight();
    }

    return node;
}

void
DecisionTree::getProbabilities(const std::vector<Pixel> *pixels, std::vector<float> &probabilities)
{
    probabilities.clear();
    probabilities.resize(grlHandIndexNum);
    std::fill(probabilities.begin(), probabilities.end(), 0.0f);

#ifdef _OPENMP
    if (pixels->size() > pixelSizeSP) {
        // Get the histogram
#pragma omp parallel shared(probabilities)
        {
            std::vector<float> tprobabilities(grlHandIndexNum);
            std::fill(tprobabilities.begin(), tprobabilities.end(), 0.0f);
#pragma omp for
            for (size_t i = 0; i < pixels->size(); ++i) {
                ++tprobabilities[(*pixels)[i].classIndex];
            }

            for (int i = 0; i < grlHandIndexNum; ++i) {
#pragma omp atomic
                probabilities[i] += tprobabilities[i];
            }
        }
    } else
#endif _OPENMP
	    for (auto it = pixels->cbegin(); it != pixels->cend(); ++it)
		    ++probabilities[it->classIndex];

    // Normalize the probabilities
    size_t sum = pixels->size();
    for (auto it = probabilities.begin(); it != probabilities.end(); ++it)
        (*it) /= sum;
}


float
DecisionTree::evaluateNode(Node *node, NodeTrainingData &data, const std::vector<cv::Mat> &depthImages)
{
    data.leftPixels->clear();
    data.leftProbabilities.clear();
    data.rightPixels->clear();
    data.rightProbabilities.clear();

#ifdef _OPENMP
    if (data.allPixels->size() > pixelSizeSP) {
        std::vector<uint8_t> directions(data.allPixels->size());

#pragma omp parallel for
        for (size_t i = 0; i < data.allPixels->size(); ++i) {
            const Pixel &p = (*data.allPixels)[i];
            const cv::Mat &depthImage = depthImages[p.imgID];
            directions[i] = node->evaluateFeature(depthImage, p);
        }

        auto itPix = data.allPixels->cbegin();
        for (auto it = directions.begin(); it != directions.end(); ++it, ++itPix) {
            if (*it == grlNodeGoLeft)
                data.leftPixels->push_back(*itPix);
            else
                data.rightPixels->push_back(*itPix);
        }
    } else
#endif // _OPENMP
        for (size_t i = 0; i < data.allPixels->size(); ++i) {
            const Pixel &p = (*data.allPixels)[i];
            const cv::Mat &depthImage = depthImages[p.imgID];
            if (node->evaluateFeature(depthImage, p) == grlNodeGoLeft)
                data.leftPixels->push_back(p);
            else
                data.rightPixels->push_back(p);
        }

    if (data.leftPixels->empty() || data.rightPixels->empty()) {
        return -std::numeric_limits<float>::infinity();
    }

    getProbabilities(data.leftPixels, data.leftProbabilities);
    getProbabilities(data.rightPixels, data.rightProbabilities);

    float allShanon = getShannonEntropy(data.allProbabilities);
    float leftEntropy = static_cast<float>(data.leftPixels->size())/data.allPixels->size() *
        getShannonEntropy(data.leftProbabilities);
    float rightEntropy = static_cast<float>(data.rightPixels->size())/data.allPixels->size() *
        getShannonEntropy(data.rightProbabilities);


    return allShanon - leftEntropy - rightEntropy;
}

void
DecisionTree::train(std::vector<Pixel> *pixels, const std::vector<cv::Mat> &depthImages,
                    int nodeTrainLimit, int maxDepth, std::mt19937 &gen, TreeTrainGPUContext *gpuContext)
{
    // For offsets
    std::uniform_int_distribution<> offsetDistribution(-learnOffsetDistr, learnOffsetDistr);
    std::uniform_real_distribution<float> thresholdDistribution(-learnThresholdDistr, learnThresholdDistr);

    NodeTrainingData data;
    data.allPixels = pixels;

#ifdef USE_GPU
    bool useGPU = gpuContext != nullptr;
    if (useGPU)
        getProbabilities(data.allPixels, data.allProbabilities, gpuContext);
    else
#endif
        getProbabilities(data.allPixels, data.allProbabilities);

    if (_root.get() != nullptr)
        _root.release();
    _root = std::make_unique<Node>(data.allPixels, data.allProbabilities);
    Node *node = _root.get();
    int depth = 1;
    int good = 0;
    while (node != nullptr) {
        // Determine if we should stay at this node, go up or go right
        if (node->getPixels() == nullptr) {
            if (!node->isLeaf() && node->getRight()->getPixels() != nullptr) {
                node = node->getRight();
                ++depth;
            } else {
                node = node->getParent();
                --depth;
            }
            continue;
        }

        ++good;

        // Get all pixels which should be split further
        data.allPixels = node->getPixels();
        data.allProbabilities = node->getProbabilities();

        printf("Training node %d at depth %d with %ju\n", good, depth, data.allPixels->size());
        fflush(stdout);
        // Set the node as leaf if max depth is achieved or the all pixels are from
        // only one class
        if (depth == maxDepth || isSingleClass(data.allProbabilities)) {
            std::cout << "Depth limit or single class at " << depth << ".\n";
            // Do not set probabilities as they should be already set
            node->setLeaf(true);
            delete data.allPixels;
            node->setPixels(nullptr);
            node = node->getParent();
            --depth;
            continue;
        }

        data.leftPixels = new std::vector<Pixel>;
        data.rightPixels = new std::vector<Pixel>;
        data.leftPixels->reserve(data.allPixels->size());
        data.rightPixels->reserve(data.allPixels->size());

        Decision bestDecision;
        std::vector<Pixel> *bestLeftPixels = nullptr;
        std::vector<Pixel> *bestRightPixels = nullptr;
        std::vector<float> bestLeftProbabilities, bestRightProbabilities;
        float bestScore = -std::numeric_limits<float>::infinity();
#ifdef _OPENMP
        const float begin_time = omp_get_wtime();
#else
        const clock_t begin_time = clock();
#endif
#ifdef USE_GPU
        if (useGPU) {
            err = gpuContext->queue.enqueueWriteBuffer(gpuContext->bufferAllPix, CL_TRUE, 0,
                                                       sizeof(Pixel)*data.allPixels->size(), data.allPixels->data());
            if (err != CL_SUCCESS) {
                std::cout << "All pix write error: \n" << err << std::endl;
                exit(-1);
            }
            err = gpuContext->getFeatureTrain.setArg(1, gpuContext->bufferAllPix);
            if (err != CL_SUCCESS) {
                std::cout << "Set all pix error: \n" << err << std::endl;
                exit(-1);
            }
            // Get all images ids which should be analyzed
            data.imageIDs.clear();
            data.imagesPixelCount.clear();
            data.imagesPixelCount.push_back(0);
            data.imageIDs.push_back(data.allPixels->front().imgID);
            for (auto it = data.allPixels->cbegin(); it != data.allPixels->cend(); ++it) {
                if (data.imageIDs.back() != it->imgID) {
                    data.imageIDs.push_back(it->imgID);
                    data.imagesPixelCount.push_back(1);
                } else {
                    ++data.imagesPixelCount.back();
                }
            }
        }
#endif
        for (int i = 0; i < nodeTrainLimit; ++i) {
            Decision decision = {
                {offsetDistribution(gen), offsetDistribution(gen)}, // u
            {offsetDistribution(gen), offsetDistribution(gen)}, // v
            thresholdDistribution(gen)}; // t
            node->setDecision(decision);
            float score;
#ifdef USE_GPU
            if (useGPU) {
                err = gpuContext->getFeatureTrain.setArg(2, sizeof(Decision), &decision);
                if (err != CL_SUCCESS) {
                    std::cout << "Set decision error: \n" << err << std::endl;
                    exit(-1);
                }
                score = evaluateNode(node, data, depthImages, gpuContext);
            } else {
#endif
                score = evaluateNode(node, data, depthImages);
#ifdef USE_GPU
            }
#endif
            if (score > bestScore) {
                bestScore = score;
                bestDecision = decision;
                delete bestLeftPixels;
                delete bestRightPixels;
                data.leftPixels->shrink_to_fit();
                data.rightPixels->shrink_to_fit();
                bestLeftPixels = data.leftPixels;
                bestRightPixels = data.rightPixels;
                data.leftPixels = new std::vector<Pixel>;
                data.rightPixels = new std::vector<Pixel>;
                bestLeftProbabilities = data.leftProbabilities;
                bestRightProbabilities = data.rightProbabilities;
            }
        }
#ifdef _OPENMP
        std::cout << float(omp_get_wtime() - begin_time) << " With score: " << bestScore << std::endl;
#else
        std::cout << float(clock() - begin_time) /  CLOCKS_PER_SEC << std::endl;
#endif

        // if didn't managed to get any score, go up and make the parent the leaf
        if (bestScore == -std::numeric_limits<float>::infinity() ||
            bestLeftPixels->empty() || bestRightPixels->empty()) {
            std::cout << "No best score at depth " << depth << ". Go up.\n";
            node = node->getParent();
            --depth;
            if (node != nullptr) {
                node->setLeaf(true);
            }
        } else {
            std::cout << "Best score " << bestScore << " at depth " << depth
                << ". Left: " << bestLeftPixels->size() << ", Right: " << bestRightPixels->size()
                << " .\n";
            std::cout << "Best decision: t = " << bestDecision.t
                << " u = (" << bestDecision.u.x << ", " << bestDecision.u.y
                << ") v = (" << bestDecision.v.x << ", " << bestDecision.v.y << ")\n";
            node->setPixels(nullptr);
            node->setDecision(bestDecision);

            node->setLeft(std::move(std::make_unique<Node>(bestLeftPixels, bestLeftProbabilities, node)));
            node->setRight(std::move(std::make_unique<Node>(bestRightPixels, bestRightProbabilities, node)));
            node = node->getLeft();
            ++depth;
            delete data.allPixels;
        }
        // Pixels no longer needed
        delete data.leftPixels;
        delete data.rightPixels;
    }
}


void
DecisionTree::readFromFile(std::ifstream & file)
{
    assert(file.is_open());

    if (_root.get() != nullptr)
        _root.release();

    _root = std::make_unique<Node>();
    Node *node = _root.get();

    while (node != nullptr) {
        char type, cmd;
        file >> type >> cmd;

        std::vector<float> probabilities;
        switch (type) {
        case '.':
            switch (cmd) {
            case 'L':
                if (node->getLeft() == nullptr)
                    node->setLeft(std::move(std::make_unique<Node>(node)));
                node = node->getLeft();
                break;
            case 'R':
                if (node->getRight() == nullptr)
                    node->setRight(std::move(std::make_unique<Node>(node)));
                node = node->getRight();
                break;
            case 'U':
                node = node->getParent();
                break;
            default:
                assert(false);
            }
            break;
        case '>':
            switch (cmd) {
            case 'L':
                // Read leaf
                file >> type;
                assert(type == '#');
                int groups;
                file >> groups;
                probabilities.resize(groups);
                for (auto it = probabilities.begin(); it != probabilities.cend(); ++it)
                    file >> *it;
                node->setProbabilities(probabilities);
                break;
            case 'D':
                // Read decision
                Decision decision;
                file >> decision.u.x >> decision.u.y
                    >> decision.v.x >> decision.v.y
                    >> decision.t;
                node->setDecision(decision);
                break;
            default:
                assert(false);
            }
            break;
        default:
            assert(false);
        }
    }
}

#ifdef USE_GPU
float
DecisionTree::evaluateNode(Node *node, NodeTrainingData &data, const std::vector<cv::Mat> &depthImages, TreeTrainGPUContext *gpuContext)
{
    int imgsProcessed = 0;
    int imgPixels = depthImages[0].cols*depthImages[0].rows;
    std::vector<float> imageData;
    imageData.resize(gpuContext->maxImages*imgPixels);
    data.leftPixels->clear();
    data.rightPixels->clear();
    data.leftPixels->reserve(data.allPixels->size());
    data.rightPixels->reserve(data.allPixels->size());
    auto itp = data.allPixels->begin();
    cl_int err;
    while (imgsProcessed < data.imageIDs.size()) {
        int constraint = std::min(static_cast<int>(data.imageIDs.size() - imgsProcessed), gpuContext->maxImages);
        int problemSize = 0;
        for (int i = 0; i < constraint; ++i) {
            int imgIndex = data.imageIDs[i];
            problemSize += data.imagesPixelCount[i];
            memcpy(imageData.data() + i*imgPixels, depthImages[imgIndex].data, imgPixels*sizeof(float));
        }
        cl::Image2DArray depthArray = cl::Image2DArray(
            gpuContext->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            cl::ImageFormat(CL_R, CL_FLOAT), constraint, depthImages[0].cols, depthImages[0].rows,
            0, 0, imageData.data(), &err);
        if (err != CL_SUCCESS) {
            std::cout << "img error " << err
                << " width " << depthImages[0].cols << " height " << depthImages[0].rows << std::endl;
            exit(-1);
        }
        err = gpuContext->getFeatureTrain.setArg(0, depthArray);
        if (err != CL_SUCCESS) {
            std::cout << "Set depth array err: \n" << err << std::endl;
            exit(-1);
        }

        err = gpuContext->getFeatureTrain.setArg(3, gpuContext->bufferSplit);
        if (err != CL_SUCCESS) {
            std::cout << "Set split buffer err: \n" << err << std::endl;
            exit(-1);
        }

        err = gpuContext->getFeatureTrain.setArg(4, sizeof(cl_int), &imgsProcessed);
        if (err != CL_SUCCESS) {
            std::cout << "Set imgs processed err: \n" << err << std::endl;
            exit(-1);
        }

        err = gpuContext->queue.enqueueNDRangeKernel(
            gpuContext->getFeatureTrain,
            cl::NullRange,
            cl::NDRange(problemSize),
            cl::NullRange);
        if (err != CL_SUCCESS) {
            std::cout << "NDRange for feature err: \n" << err << std::endl;
            exit(-1);
        }

        std::vector<cl_char> split(problemSize);
        err = gpuContext->queue.enqueueReadBuffer(gpuContext->bufferSplit, CL_TRUE, 0,
                                                  sizeof(cl_char)*problemSize, split.data());
        if (err != CL_SUCCESS) {
            std::cout << "read split error " << err << std::endl;
            exit(-1);
        }

        for (auto its = split.cbegin(); its != split.cend(); ++its, ++itp) {
            if (*its == -1)
                data.leftPixels->push_back(*itp);
            else
                data.rightPixels->push_back(*itp);
        }

        imgsProcessed += constraint;
    }

    if (data.leftPixels->empty() || data.rightPixels->empty()) {
        return -std::numeric_limits<float>::infinity();
    }

    getProbabilities(data.leftPixels, data.leftProbabilities, gpuContext);
    getProbabilities(data.rightPixels, data.rightProbabilities, gpuContext);

    float allShanon = getShannonEntropy(data.allProbabilities);
    float leftEntropy = static_cast<float>(data.leftPixels->size())/data.allPixels->size() *
        getShannonEntropy(data.leftProbabilities);
    float rightEntropy = static_cast<float>(data.rightPixels->size())/data.allPixels->size() *
        getShannonEntropy(data.rightProbabilities);


    return  -allShanon - leftEntropy - rightEntropy;
}


void
DecisionTree::getProbabilities(const std::vector<Pixel> *pixels, std::vector<float> &probabilities, TreeTrainGPUContext *gpuContext)
{
    cl_int err = gpuContext->queue.enqueueFillBuffer<cl_uint>(gpuContext->bufferPixCount, 0, 0,
                                                              sizeof(cl_uint)*grlHandIndexNum);
    if (err != CL_SUCCESS) {
        std::cout << "Fill index num err: \n" << err << std::endl;
        exit(-1);
    }
    err = gpuContext->queue.enqueueWriteBuffer(gpuContext->bufferPix, CL_TRUE, 0,
                                               sizeof(Pixel)*pixels->size(), pixels->data());
    if (err != CL_SUCCESS) {
        std::cout << "Set depth array err: \n" << err << std::endl;
        exit(-1);
    }
    err = gpuContext->getProbabilities.setArg(0, gpuContext->bufferPix);
    if (err != CL_SUCCESS) {
        std::cout << "Set arg buffer pix err: \n" << err << std::endl;
        exit(-1);
    }
    err = gpuContext->getProbabilities.setArg(1, gpuContext->bufferPixCount);
    if (err != CL_SUCCESS) {
        std::cout << "Set arg buffer pix count: \n" << err << std::endl;
        exit(-1);
    }
    err = gpuContext->queue.enqueueNDRangeKernel(
        gpuContext->getProbabilities,
        cl::NullRange,
        cl::NDRange(pixels->size()),
        cl::NullRange);
    if (err != CL_SUCCESS) {
        std::cout << "NDRange for prob error: \n" << err << std::endl;
        exit(-1);
    }

    std::vector<cl_uint> countResults(grlHandIndexNum);
    err = gpuContext->queue.enqueueReadBuffer(gpuContext->bufferPixCount, CL_TRUE, 0,
                                              sizeof(cl_uint)*grlHandIndexNum, countResults.data());
    if (err != CL_SUCCESS) {
        std::cout << "Reading pix count error: \n" << err << std::endl;
        exit(-1);
    }
    probabilities.resize(grlHandIndexNum);
    // Normalize the probabilities
    size_t sum = pixels->size();
    auto itc = countResults.cbegin();
    for (auto itp = probabilities.begin(); itp != probabilities.end(); ++itp, ++itc)
        (*itp) = (*itc)/static_cast<float>(sum);
}
#endif

const std::vector<float> &
DecisionTree::classifyPixel(const cv::Mat &depthImage, const Pixel &p)
{
    const Node *leaf = _root->getLeafForPixel(depthImage, p);

    return leaf->getProbabilities();
}

}
