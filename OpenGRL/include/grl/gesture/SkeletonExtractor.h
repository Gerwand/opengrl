#pragma once

#include <grl/gesture/GestureExtractor.h>
#include <grl/gesture/FloodFillClipped.h>

namespace grl {

/**
 * Configuration for the SkeletonExtractor. Adds paramemter for the depth
 * tolerance for flood fill extraction.
 */
struct SkeletonExtractorConfig : ExtractorConfig
{
    // Tolerance of the depth for FloodFill.
    int depthTolerance;
};

/**
 * Class responsible for extracting gestures using data from skeleton and depth
 * image.
 */
class SkeletonExtractor : public GestureExtractor
{
public:
    /**
     * Initialize the SkeletonExtractor.
     * The config should be of type SkeletonExtractorConfig which is adding
     * additional configuration options.
     *
     * @param config configuration for the object.
     * @returns true if the object was intiialized successfully
     */
    bool init(const SkeletonExtractorConfig &config);

protected:
    /**
     * Check if the hand is visiable on the image and can be extraced with the
     * given input data.
     *
     * @param side Hand (left/right) which should be checked.
     * @param depthImage depth image of the hands.
     * @param skeleton skeleton of the person detected on the image.
     * @returns true if the hand can be further extracted, false otherwise.
     */
    virtual bool isHandValid(Side side, const cv::Mat &depthImage, const Skeleton &skeleton) const override;

    /**
     * Extract the hand using the depth image and the skeleton.
     * The flood fill is used for extracting the hand object.
     *
     * @param side[in] Hand (left/right) which should be checked.
     * @param depthImagep[in] depth image of the hands.
     * @param skeleton[in] skeleton of the person detected on the image.
     * @param hand[out] object representing hand, to which the hand will be
     * exported.
     */
    virtual void extractHand(Side side, const cv::Mat &depthImage, const Skeleton &skeleton, DepthObject     &hand) override;

private:
    // Configuration of the extractor.
    SkeletonExtractorConfig _config;
    // Flood fill which is extracting the object on the image with possibility
    // to use the plane for taking in only some voxels (in front of the plane).
    FloodFillClipped _ff;
};

inline bool
SkeletonExtractor::init(const SkeletonExtractorConfig &config)
{
    if (!GestureExtractor::init(config))
        return false;

    _config = config;

    return true;
}

}
