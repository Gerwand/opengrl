#pragma once

#include <grl/gesture/GestureExtractor.h>
#include <grl/gesture/FloodFillClipped.h>

namespace grl {

struct SkeletonExtractorConfig : ExtractorConfig
{
	int tolerance;
};

class SkeletonExtractor : public GestureExtractor
{
public:
	virtual bool init(const ExtractorConfig &config) override;
	virtual void extractHands(const cv::Mat &depthImage, const Skeleton &skeleton, Object &leftHand, Object &rightHand) override;

private:
	SkeletonExtractorConfig _config;
	FloodFillClipped _floodFillRight;
	FloodFillClipped _floodFillLeft;
};

inline bool 
SkeletonExtractor::init(const ExtractorConfig &config)
{
	const SkeletonExtractorConfig &skeletonConfig =
		static_cast<const SkeletonExtractorConfig &>(config);

	_config = skeletonConfig;

	return true;
}


}