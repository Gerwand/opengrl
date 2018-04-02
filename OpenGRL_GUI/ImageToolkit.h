#pragma once

#include <opencv/cxcore.hpp>

namespace grl {

void EnhanceDepthFrame(cv::Mat& image);
cv::Mat DepthToColor(const cv::Mat& depthFrame);
void EnhanceExtractedHand(cv::Mat& image);

inline void
EnhanceDepthFrame(cv::Mat& image)
{
	cv::medianBlur(image, image, 3);
}


inline cv::Mat
DepthToColor(const cv::Mat& depthFrame)
{
	cv::Mat colorImage;

	cv::Mat tmp;
	depthFrame.copyTo(tmp);

	int el = tmp.cols * tmp.rows;

	uint16_t *data = reinterpret_cast<uint16_t *>(tmp.data);
	for (int i = 0; i < el; ++i) {
		if (*data > 2000)
			*data = 2000;
		else if (*data < 1000)
			*data = 1000;
		++data;
	}

	cv::normalize(tmp, colorImage, 0, UINT8_MAX, cv::NORM_MINMAX, CV_8UC1);
	//colorImage.convertTo(colorImage, CV_8UC1);
	colorImage = UINT8_MAX - colorImage; // Invert colors (white will be the closest)
	//cv::applyColorMap(colorImage, colorImage, cv::COLORMAP_WINTER); // Create colormap

	return colorImage;
}

inline void
EnhanceExtractedHand(cv::Mat& image)
{
	static cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(image, image, cv::MORPH_OPEN, element);
}

}