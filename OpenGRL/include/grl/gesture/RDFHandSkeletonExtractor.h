#pragma once

#include <grl/gesture/HandSkeletonExtractor.h>
#include <grl/rdf/RandomDecisionForest.h>

namespace grl {

// Implementation of the extractor using the Random Decision Forest structure.
class RDFHandSkeletonExtractor : public HandSkeletonExtractor
{
public:
    // Load the RDF from the file. Optionally, the object representing the
    // camera can be passed. It will cause the object to determine world
    // coordinates using the camera space of the camera.
    bool init(const std::string &fileWithRDF, const DepthCamera *camera = nullptr);

    void extractSkeleton(const grl::DepthObject &hand, grl::HandSkeleton &handSkeleton) override;

    // Just for debug and data presentation, it returns image with hand classes.
    // However, this is not the RGB image, the classes contains values like 1, 2, 3, etc.
    // representing the class ID. It must be further converted.
    cv::Mat getLastClasses() { return _lastClasses; }

private:
    using JointsApproximation = std::array<grl::HandJoint, grl::grlHandIndexNum>;

    cv::Mat _lastClasses;
    grl::RandomDecisionForest _forest;
    const grl::DepthCamera *_camera;

    // Density estimator using the gaussian kernel. It returns the gradient and
    // the certainty of the location. The certainty is calculated using the sum
    // of probabilities in the kernel area.
    std::pair<cv::Point2i, float> densityEstimator(
        const cv::Mat &weights,
        const grl::GaussianKernel &kernel,
        cv::Point centerPoint);

    void approximateJoints(
        const cv::Mat &depthImage,
        const grl::ClassesWeights &weights,
        grl::HandSkeleton &skeleton,
        const grl::ClassesPoints &bestProbabilities);

    // The depth camera is returning depth frame in uint16 representing millimeters,
    // while the RDF is using floats representing meters.
    static void convertDepthForRDF(const DepthObject &hand, cv::Mat &convertedDepth);

    // Big kernel for big hand parts
    static const GaussianKernel _kernelBig;
    // Small kernel for small parts, like fingers
    static const GaussianKernel _kernelSmall;

    // Map of the kernels for each class
    static const std::array<const GaussianKernel *, grlHandIndexNum> _kernels;

    // For now, let's assume it's 10 but it should be calculated how at 1 meter
    // how wide is 1 pixel (wide of the hand at 1 meter depth). Used only
    // if no camera is supplied.
    static constexpr float _depthFactor = 10.0f;
};

}
