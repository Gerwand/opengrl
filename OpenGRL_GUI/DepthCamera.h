#pragma once

#include "MathUtils.h"

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
	bool tracked;
	Vec2i coordDepthImage;
	Vec3f coordWorld;
	Vec3f orientation;
};

struct Skeleton {
	Joint joints[JointType::COUNT];
	float lean;
	float distance;
};

typedef std::vector<Skeleton> Skeletons;

class DepthCamera
{
public:
	virtual int init() = 0;
	virtual void destroy() = 0;

	virtual bool getFrame(cv::Mat *image) = 0;
	virtual bool getSkeletons(Skeletons *skeletons) = 0;

    virtual bool worldToImage(const std::vector<Vec3f> &world, std::vector<Vec2f> &image) const = 0;
    virtual bool worldToImage(Vec3f world, Vec2f &image) const = 0;

	void getSize(int &width, int &height) const;

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