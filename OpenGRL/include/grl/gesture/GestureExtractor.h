#pragma once

#include <grl/camera/DepthCamera.h>

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>

namespace grl {

struct Voxel
{
	Vec3i coords;
};

struct Object
{
	std::vector<const Voxel *> voxels;
	int size;
	int minVal = INT_MAX;
	int maxVal = INT_MIN;

	uint8_t accuracy; // how good the object is detected

	cv::Rect boundingBox = cv::Rect(INT_MAX, INT_MAX, 0, 0);
	cv::Mat image;

	void generateImage();
	void putVoxel(Voxel *voxel);

    void reset();

private:
	// Helpers for defining width and height
	int _maxX = INT_MIN;
	int _maxY = INT_MIN;
};

struct ExtractorConfig {};

class GestureExtractor
{
public:
	virtual bool init(const ExtractorConfig &config) = 0;
	virtual void extractHands(const cv::Mat &depthImage, const Skeleton &skeleton, Object &leftHand, Object &rightHand) = 0;
};

};
