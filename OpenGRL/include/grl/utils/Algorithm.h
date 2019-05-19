#pragma once

#include <opencv/cv.hpp>

namespace grl {

class Algorithm {
public:
	virtual cv::Mat run(const cv::Mat& image) = 0;
	cv::Mat operator()(const cv::Mat &image) { return run(image); };
};

}