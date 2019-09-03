#pragma once

#include <grl/utils/MathUtils.h>

#include <opencv/cv.hpp>

namespace grl {

enum JointType
{
	LEFT_HAND = 0,
	LEFT_WRIST,
	LEFT_ELBOW,
	LEFT_SHOULDER,
	RIGHT_HAND,
	RIGHT_WRIST,
	RIGHT_ELBOW,
	RIGHT_SHOULDER,
	SPINE_BASE,
	SPINE_SHOULDERS,
	COUNT
};

struct Joint {
	JointType type;
    // Indicates if the joint was traced by the camera
	bool tracked;
    // 2D coords on the depth map
	Vec2i coordDepthImage;
    // 3D coords in the depth space
	Vec3f coordWorld;
    // Joint orientation, as read from the device
	Vec3f orientation;
};

struct Skeleton {
	Joint joints[JointType::COUNT];
    // Lean in the X or Y axis (bigger one is taken)
	float lean;
    // Distance from the camera
	float distance;
};

typedef std::vector<Skeleton> Skeletons;

class DepthCamera
{
public:
	virtual int init() = 0;
	virtual void destroy() = 0;

    // Get the depth frame from the device
	virtual bool getFrame(cv::Mat *image) = 0;
    // Get all of the skeleton detected by the device
	virtual bool getSkeletons(Skeletons *skeletons) = 0;

    // Convert multiple coordinates from the world to the depth camera space (pixels)
    virtual bool worldToImage(const std::vector<Vec3f> &world, std::vector<Vec2f> &image) const = 0;
    // Convert coordinate from the world to the depth camera space (pixel)
    virtual bool worldToImage(Vec3f world, Vec2f &image) const = 0;
    // Opposite to the worlToImage
    virtual bool imageToWorld(const std::vector<Vec2f> &image, const std::vector<uint16_t> &depths, std::vector<Vec3f> &world) const = 0;
    virtual bool imageToWorld(Vec3f &world, uint16_t depth, Vec2f image) const = 0;

    // Get size of the depth frame
	void getSize(int &width, int &height) const;

    // Indicates if the object was initialized successfully
	bool isValid() const;

protected:
	int _depthWidth = 0;
	int _depthHeight = 0;

	cv::Mat _lastFrame;

	void setValid(bool valid);

private:
	bool _valid = false;
};

inline bool
DepthCamera::isValid() const
{
	return _valid;
}

inline void
DepthCamera::setValid(bool valid)
{
	_valid = valid;
}

inline void
DepthCamera::getSize(int &width, int &height) const
{
	width = _depthWidth;
	height = _depthHeight;
}


}