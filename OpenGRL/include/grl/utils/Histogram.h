#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#define COLOR_MIN 0.0f
#define COLOR_MAX 256.0f
#define DEFAULT_SIZE 255

namespace grl {

class Histogram
{
public:
	enum Channels
	{
		GRAY = CV_8UC1,
		RGB = CV_8UC3,
		RGBA = CV_8UC4
	};

	Histogram(cv::Mat mask = cv::Mat());

	cv::Mat getHistogram(const cv::Mat& image);
	virtual cv::Mat getHistogramImage(const cv::Mat& image, int scale = 1);
	virtual cv::Mat getHistogramAsImage(const cv::Mat& hist, int scale = 1) = 0;

protected:
	virtual void init(int bins) = 0;

	typedef std::vector<int> intVector;
	typedef std::vector<float> floatVector;
	typedef std::vector<float*> floatPtrVector;

	cv::Mat _histogram;
	cv::Mat _mask;

	intVector _channels;
	intVector _histSize;
	floatVector _range;
	floatPtrVector _ranges;
	int _dims;

	static const int _imgSize = 300;
	static const int _num = 1;
	static bool _uniform;
	static bool _accumulate;
};


inline
Histogram::Histogram(cv::Mat mask) : _mask(mask)
{
}

inline cv::Mat
Histogram::getHistogram(const cv::Mat& image)
{
	cv::Mat hist;
	calcHist(&image, 1, _channels.data(), _mask, hist, _dims, _histSize.data(),
		(const float**)_ranges.data(), _uniform, _accumulate);

	return hist;
}

inline cv::Mat
Histogram::getHistogramImage(const cv::Mat& image, int scale)
{
	cv::Mat hist = getHistogram(image);
	return getHistogramAsImage(hist, scale);
}

}