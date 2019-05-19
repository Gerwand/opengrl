#pragma once

// Legacy code - not supported for now
#if 0

#include <opencv2\core.hpp>

namespace grl {

struct AnalyzerConfig {};

class GestureAnalyzer
{
public:
	virtual void init(const AnalyzerConfig *config) = 0;

	virtual void extractObjects(const cv::Mat &depthImage) = 0;
	virtual bool findHands(cv::Mat& handsImage) = 0;
	virtual void drawMarkers(cv::Mat& destination) = 0;
};

}

#endif // Legacy code
