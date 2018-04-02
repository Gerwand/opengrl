#pragma once

#include <string>
#include <map>
#include <iostream>

#include "GestureMatcher.h"

namespace grl {

class ORBMatcher : public GestureMatcher
{
private:
	typedef GestureMatcher super;

public:
	bool init(int nfeatures = 1000);

	bool addGesture(const std::string& name, const std::string& filePath);
	virtual void addGesture(const std::string& name, cv::Mat image) override;

	virtual void learn();
	virtual Match matchBestGesture(cv::Mat& image) override;
private:
	struct Gesture
	{
		cv::Mat image;
		cv::Ptr<cv::DescriptorMatcher> matcher;
	};

	std::map<std::string, Gesture> _gestures;

	cv::Ptr<cv::Feature2D> _detector;

	double _ratioT = 0.6;
};

inline bool ORBMatcher::init(int nfeatures)
{
	if (!super::init())
		return false;
	_detector = cv::ORB::create(nfeatures);

	return true;
}

}