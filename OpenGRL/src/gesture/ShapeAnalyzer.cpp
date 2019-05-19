// Legacy code - not supported for now
#if 0

#include "ShapeAnalyzer.h"

namespace grl {

void
ShapeAnalyzer::extractObjects(const cv::Mat& depthImage)
{
	_maskedDepth = depthImage.clone();

	// A little awkward conditionals, that can improve performance,
	// especially with big pictures
	if (_config.minDist > 1 && _config.maxDist > 0) {
		for (auto it = _maskedDepth.begin<depth_t>();
			 it != _maskedDepth.end<depth_t>(); ++it) {
			depth_t val = *it;
			if (val < _config.minDist || val > _config.maxDist)
				*it = 0;
		}
	} else if (_config.minDist > 1) {
		for (auto it = _maskedDepth.begin<depth_t>();
			 it != _maskedDepth.end<depth_t>(); ++it) {
			depth_t val = *it;
			if (val < _config.minDist)
				*it = 0;
		}
	} else if (_config.maxDist > 0) {
		for (auto it = _maskedDepth.begin<depth_t>();
			 it != _maskedDepth.end<depth_t>(); ++it) {
			depth_t val = *it;
			if (val > _config.maxDist)
				*it = 0;
		}
	}

	_objects.clear();
	_ff.getObjects(_maskedDepth, _objects);
}

bool
ShapeAnalyzer::findHands(cv::Mat& destination)
{
	// Analyze objects
	Object *handObject = nullptr;
	bool collision = false;
	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		// Properties of the object, that should describe whole arm
		if (it->roundness < _config.roundness &&
			it->circularity < _config.circularity && 
			it->aspectRatio < _config.aspectRatio) {
			destination = cv::Mat::zeros(_maskedDepth.size(), CV_8UC1);
			// Draw object on the earlier created image
			if (handObject != nullptr) {
				DEBUG_INFO("Multiple hands detected");
				collision = true;
			} else {
				handObject = &(*it);
			}
		}
	}

	// If there were many "hand-like objects" or none
	if (collision || handObject == nullptr)
		return false;

	handObject->drawOnImage(destination);
	cv::cvtColor(destination, destination, cv::COLOR_GRAY2BGR);

	return true;
}

}

#endif // Legacy code
