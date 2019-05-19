#pragma once

// Legacy code - not supported for now
#if 0

#include "GestureAnalyzer.h"
#include "FloodFill.h"
#include "DebugTools.h"

namespace grl {

typedef uint16_t depth_t;

struct ShapeConfig : public AnalyzerConfig
{
	int minObjectSize = 0;
	int maxTolerance = 10;
	depth_t minDist = 0;
	depth_t maxDist = 0;
	double roundness = 0.3;
	double circularity = 0.4;
	double aspectRatio = 0.5;
};

class ShapeAnalyzer : public GestureAnalyzer
{
public:
	typedef grl::Object<depth_t> Object;
	typedef std::vector<Object> Objects;

	virtual void init(const AnalyzerConfig *config) override;
	virtual void extractObjects(const cv::Mat& depthImage)  override;
	virtual bool findHands(cv::Mat& handsImage) override;
	virtual void drawMarkers(cv::Mat& destination) override;

private:
	ShapeConfig _config;

	FloodFill<depth_t> _ff;
	Objects _objects;

	cv::Mat _maskedDepth;
};

inline void
ShapeAnalyzer::init(const AnalyzerConfig *config)
{
	const ShapeConfig *shapeConfig = static_cast<const ShapeConfig *>(config);

	_config = *shapeConfig;

	_ff.setup(_config.minObjectSize, _config.maxTolerance);
}


inline void
ShapeAnalyzer::drawMarkers(cv::Mat& destination)
{
	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		//Draw rectangles and ellipses for all objects
		cv::rectangle(destination, it->box, cv::Scalar(0, 0, 255));
		cv::ellipse(destination, it->ellipse, cv::Scalar(0, 0, 255));
	}
}

}

#endif // Legacy code
