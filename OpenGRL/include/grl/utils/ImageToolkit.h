#pragma once

#include <grl/camera/DepthCamera.h>
#include <grl/track/TrackPoints.h>
#include <grl/gesture/HandSkeleton.h>

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
DepthToColor(const cv::Mat& depthFrame, uint16_t rangeMin, uint16_t rangeMax)
{
	cv::Mat colorImage;

	cv::Mat tmp;
	depthFrame.copyTo(tmp);

	int el = tmp.cols * tmp.rows;

	uint16_t *data = reinterpret_cast<uint16_t *>(tmp.data);
	for (int i = 0; i < el; ++i) {
		if (*data > rangeMax)
			*data = rangeMax;
		else if (*data < rangeMin)
			*data = rangeMin;
		++data;
	}

	cv::normalize(tmp, colorImage, 0, UINT8_MAX, cv::NORM_MINMAX, CV_8UC1);
	colorImage = UINT8_MAX - colorImage; // Invert colors (white will be the closest)

	return colorImage;
}

inline void
EnhanceExtractedHand(cv::Mat& image)
{
	static cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(image, image, cv::MORPH_OPEN, element);
}

void drawSimpleSkeleton(const Skeleton &skeleton, cv::Mat &image);
void drawTrack(const TrackPoints &track, cv::Mat &image, const DepthCamera &camera);
void drawJointsOnImage(const HandSkeleton &skeleton, cv::Mat &image, float certainty = 0.1f);

}