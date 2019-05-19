#pragma once

#include <grl/utils/Histogram.h>
#include <vector>

namespace grl {

struct Peakeness
{
	int width;
	float height;
	int maxPos;
	float value;
	int colLeft;
	int colRight;
};

class HistogramGray : public Histogram
{
public:
	enum CompareMethod
	{
		CORRELATION = CV_COMP_CORREL,
		CHI_SQUARE = CV_COMP_CHISQR,
		INTERSECTION = CV_COMP_INTERSECT,
		BHATTACHARYYA = CV_COMP_BHATTACHARYYA
	};

	HistogramGray(int bins);

	virtual cv::Mat getHistogramAsImage(const cv::Mat& hist, int scale = 1) override;

	static void smooth(cv::Mat& hist, int width = 3);
	static double compare(const cv::Mat& hist1, const cv::Mat& hist2, CompareMethod method);
	static std::vector<Peakeness> getPeakenessList(const cv::Mat& hist1);
	static Peakeness getPeakeness(const cv::Mat& hist1, int position);
	static double getMin(const cv::Mat& hist);
	static double getMax(const cv::Mat& hist);
	static double getMean(const cv::Mat& hist);
	static void normalize(cv::Mat& hist);
	static void equalize(cv::Mat& image);
	static void stretch(cv::Mat& image);

protected:
	virtual void init(int bins) override;
};

inline
HistogramGray::HistogramGray(int bins)
{
	init(bins);
}

inline void
HistogramGray::smooth(cv::Mat& hist, int width)
{
	cv::blur(hist, hist, cv::Size(1, width));
}

inline double
HistogramGray::compare(const cv::Mat& hist1, const cv::Mat& hist2, CompareMethod method)
{
	return compareHist(hist1, hist2, method);
}

inline double
HistogramGray::getMin(const cv::Mat& hist)
{
	double min;
	cv::minMaxLoc(hist, &min);
	return min;
}

inline double
HistogramGray::getMax(const cv::Mat& hist)
{
	double max;
	cv::minMaxLoc(hist, nullptr, &max);
	return max;
}

inline double
HistogramGray::getMean(const cv::Mat& hist)
{
	return cv::mean(hist)[0];
}

inline void
HistogramGray::normalize(cv::Mat& hist)
{
	cv::normalize(hist, hist);
}

inline void
HistogramGray::equalize(cv::Mat& image)
{
	cv::equalizeHist(image, image);
}

inline void
HistogramGray::stretch(cv::Mat& image)
{
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}

}