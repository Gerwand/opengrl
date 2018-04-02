#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv/cv.hpp>

namespace grl {

class GestureMatcher
{
public:
	struct Match
	{
		std::string name = std::string("Invalid");
		double matches = -1.0;
		cv::Mat image = cv::Mat(0, 0, CV_8UC1);
	};

	virtual bool init();
	virtual void addGesture(const std::string& name, cv::Mat image) = 0;

	virtual void learn() = 0;
	virtual Match matchBestGesture(cv::Mat& image) = 0;
};

inline bool GestureMatcher::init()
{
	return true;
}

}