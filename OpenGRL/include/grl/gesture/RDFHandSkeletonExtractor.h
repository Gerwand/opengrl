#pragma once

#include <grl/gesture/HandSkeletonExtractor.h>
#include <grl/rdf/RandomDecisionForest.h>

namespace grl {

class RDFHandSkeletonExtractor : public HandSkeletonExtractor
{
public:
    bool init(const std::string &fileWithRDF, const DepthCamera *camera = nullptr);

    void extractSkeleton(const grl::DepthObject &hand, grl::HandSkeleton &handSkeleton) override;

    cv::Mat getLastClasses() { return _lastClasses; }

private:
    using JointsApproximation = std::array<grl::HandJoint, grl::grlHandIndexNum>;

    cv::Mat _lastClasses;
    grl::RandomDecisionForest _forest;
    const grl::DepthCamera *_camera;

    std::pair<cv::Point2i, float> densityEstimator(
        const cv::Mat &weights,
        const grl::GaussianKernel &kernel,
        cv::Point centerPoint);

    void approximateJoints(
        const cv::Mat &depthImage,
        const grl::ClassesWeights &weights,
        grl::HandSkeleton &skeleton,
        const grl::ClassesPoints &bestProbabilities);

    static void convertDepthForRDF(const DepthObject &hand, cv::Mat &convertedDepth);

    static const GaussianKernel _kernelBig;
    static const GaussianKernel _kernelSmall;

    static const std::array<const GaussianKernel *, grlHandIndexNum> _kernels;

    // For now, let's assume it's 10 but it should be calculated how at 1 meter
    // how wide is 1 pixel (wide of the hand at 1 meter depth)
    static constexpr float _depthFactor = 10.0f;
};

}
