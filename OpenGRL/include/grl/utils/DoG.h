#pragma once


#include <grl/utils/Algorithm.h>
#include <grl/utils/HistogramGray.h>

namespace grl {

class DoG : public Algorithm
{
public:
	DoG(int size, double s1, double s2);
	virtual cv::Mat run(const cv::Mat& image) override;

private:
	int _size;
	double _s1;
	double _s2;
};

inline
DoG::DoG(int size, double s1, double s2)
{
	_size = size;
	_s1 = s1;
	_s2 = s2;
}

inline cv::Mat
DoG::run(const cv::Mat& image)
{
	cv::Mat details, background, result;

	cv::Mat imgCopy = image.clone();
	//image.convertTo(imgCopy, CV_32F);

	cv::GaussianBlur(image, details, cv::Size(_size, _size), _s1);
	cv::GaussianBlur(image, background, cv::Size(_size, _size), _s2);

	cv::subtract(details, background, result);
	cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);

	HistogramGray::stretch(result);

	return result;
}

}