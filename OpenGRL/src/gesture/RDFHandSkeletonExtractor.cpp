#include <grl/gesture/RDFHandSkeletonExtractor.h>

namespace grl {

const GaussianKernel RDFHandSkeletonExtractor::_kernelBig(25.0f, 51);
const GaussianKernel RDFHandSkeletonExtractor::_kernelSmall(12.0f, 25);

const std::array<const grl::GaussianKernel *, grl::grlHandIndexNum> RDFHandSkeletonExtractor::_kernels = {
    &_kernelBig, //Wrist
    &_kernelBig, //Center
    &_kernelSmall, //Thumb base
    &_kernelSmall, //Thumb mid
    &_kernelSmall, //Thumb top
    &_kernelSmall, //Thumb tip
    &_kernelSmall, //Index base
    &_kernelSmall, //Index mid
    &_kernelSmall, //Index top
    &_kernelSmall, //Index tip
    &_kernelSmall, //Middle base
    &_kernelSmall, //Middle mid
    &_kernelSmall, //Middle top
    &_kernelSmall, //Middle tip
    &_kernelSmall, //Ring base
    &_kernelSmall, //Ring mid
    &_kernelSmall, //Ring top
    &_kernelSmall, //Ring tip
    &_kernelSmall, //Pinky base
    &_kernelSmall, //Pinky mid
    &_kernelSmall, //Pinky top
    &_kernelSmall, //Pinky tip
};

bool RDFHandSkeletonExtractor::init(const std::string &fileWithRDF, const DepthCamera *camera)
{
    _camera = camera;
    return _forest.loadFromFile(fileWithRDF);
}

void RDFHandSkeletonExtractor::extractSkeleton(const grl::DepthObject &hand, grl::HandSkeleton &handSkeleton)
{
    grl::ClassesWeights weights;
    grl::ClassesPoints bestProbabilities;
    cv::Mat depthForRDF;
    convertDepthForRDF(hand, depthForRDF);

    _forest.classifyImage(depthForRDF, _lastClasses, weights, bestProbabilities);
    approximateJoints(depthForRDF, weights, handSkeleton, bestProbabilities);
}


std::pair<cv::Point2i, float> RDFHandSkeletonExtractor::densityEstimator(
    const cv::Mat &weights,
    const GaussianKernel &kernel,
    cv::Point centerPoint)
{
    int centerOffsets = static_cast<int>(kernel.getSize() / 2);
    int xStart = centerPoint.x - centerOffsets;
    int yStart = centerPoint.y - centerOffsets;
    int xEnd = centerPoint.x + centerOffsets;
    int yEnd = centerPoint.y + centerOffsets;

    // Limit the image to the size of the kernel
    cv::Rect roi(
        clampMin(xStart, 0),
        clampMin(yStart, 0),
        clampMax(xEnd, weights.cols) - clampMin(xStart, 0),
        clampMax(yEnd, weights.rows) - clampMin(yStart, 0));
    const cv::Mat &weightsROI = weights(roi);

    // Count the amount of the pixels that had to be skipped from each side
    int skipLeft = clampMin(-xStart, 0);
    //int skipRight = grl::clampMin(xEnd - weights.cols, 0);
    int skipTop = clampMin(-yStart, 0);
    //int skipBottom = grl::clampMin(yEnd - weights.rows, 0);

    float sum = 0.0f;
    float gradientX = 0.0f;
    float gradientY = 0.0f;
    // y - ROI coordinates
    // ky - kernel y coordinate (it can be different because of the borders)
    // realY - image y coordinate
    for (int y = 0, ky = skipTop, realY = roi.y;
         y < weightsROI.rows;
         ++ky, ++y, ++realY)
    {
        // Definition of the variables same as for y
        for (int x = 0, kx = skipLeft, realX = roi.x;
             x < weightsROI.cols;
             ++kx, ++x, ++realX)
        {
            float kval = kernel(kx, ky);
            float wval = weightsROI.at<float>(cv::Point(x, y));
            float value = wval * kval;
            // Gradient is calculated using weight(roi_x,roi_y)*gauss(roi_x,roi_y)*x
            // but because roi can be smaller then the kernel we must use separate counters.
            gradientX += value * realX;
            gradientY += value * realY;
            sum += value;
        }
    }

    // Check if there density is higher > 0
    std::pair<cv::Point2i, float> result;
    if (sum > 0.0f) {
        // If so, calculate the average movement for X and Y + density result
        result = std::make_pair(
            cv::Point(static_cast<int>(gradientX / sum),
                      static_cast<int>(gradientY / sum)),
            static_cast<float>(cv::sum(weightsROI)[0]) // Take as a certainty sum of the probabilities
                                // of pixels creating the mode.
        );
    } else {
        // Otherwise, to prevent divide by 0, assign the 0 manually
        result = std::make_pair(
            cv::Point(centerPoint.x, centerPoint.y),
            0.0f
        );
    }

    return result;
}

void RDFHandSkeletonExtractor::approximateJoints(
    const cv::Mat &depthImage,
    const grl::ClassesWeights &weights,
    grl::HandSkeleton &skeleton,
    const grl::ClassesPoints &bestProbabilities)
{
    for (size_t i = 0; i < grl::grlHandIndexNum; ++i) {
        // Receive the joint type that the given class is approximating
        grl::HandJointType jointIndex = RDFTools::handClassToHandJoint(static_cast<grl::HandClass>(i));
        if (jointIndex == grl::grlHandJointUnknown)
            continue;

        // Get the weights for the given class
        const cv::Mat &classWeights = weights[i];
        // Get the kernel for the given class (bigger parts have bigger kernel)
        const grl::GaussianKernel &kernel = *_kernels[i];

        // We are taking few points that had the best probability with the given
        // class to start looking for joints there.
        const std::multimap<float, Vec2i> &startingPoints = bestProbabilities[i];

        grl::HandJoint &joint = skeleton[jointIndex];
        std::pair<cv::Point2i, float> densityResult;
        for (auto it = startingPoints.begin(); it != startingPoints.end(); ++it) {
            // Start with this point
            cv::Point centerPoint(it->second.x, it->second.y);

            // Adjust the location using density estimator until it won't change
            cv::Point oldPoint;
            do {
                oldPoint = centerPoint;
                densityResult = densityEstimator(classWeights, kernel, centerPoint);
                centerPoint = densityResult.first;
            } while (oldPoint != centerPoint);

            joint.certainty = densityResult.second;

            float depth = depthImage.at<float>(densityResult.first);
            if (_camera != nullptr) {
                Vec2f image(static_cast<float>(densityResult.first.x), 
                            static_cast<float>(densityResult.first.y));
                _camera->imageToWorld(joint.location, static_cast<uint16_t>(depth*1000), image);
            } else {

            // Save the location by normalizing it by the depth and constant value
                joint.location = grl::Vec3f(
                    static_cast<float>(densityResult.first.x)/(depth*_depthFactor),
                    static_cast<float>(densityResult.first.y)/(depth*_depthFactor),
                    depth
                );
            }
            joint.locationImage = grl::Vec2i(densityResult.first.x,
                                             densityResult.first.y);
            joint.type = jointIndex;
        }
    }
}


void RDFHandSkeletonExtractor::convertDepthForRDF(
    const grl::DepthObject &hand,
    cv::Mat &convertedDepth)
{
    const cv::Rect &handSize = hand.getBoundingBox();
    // The depth camera is using as a unit milimeters in uint16_t, while the RDF is
    // using as a unit meters in a float.
    convertedDepth = cv::Mat::zeros(handSize.height, handSize.width, CV_32FC1);


    for (auto itVoxel = hand.getVoxels().cbegin();
         itVoxel != hand.getVoxels().cend();
         ++itVoxel)
    {
        const Voxel *voxel = *itVoxel;
        // Convert milimeters to meters
        convertedDepth.at<float>(cv::Point(voxel->coords.x - handSize.x, voxel->coords.y - handSize.y)) =
            static_cast<float>(voxel->coords.z)/1000.0f;
    }
}


}
