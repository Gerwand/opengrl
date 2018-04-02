#pragma once

#define _USE_MATH_DEFINES

#include <KinectSDK20/Kinect.h>

#include "DepthCamera.h"
#include "MathUtils.h"
#include "DebugTools.h"

namespace grl {

class KinectCamera : public DepthCamera
{
public:
	~KinectCamera();

	virtual int init() override;
	virtual void destroy() override;

	virtual bool getFrame(cv::Mat *image) override;
	virtual bool getSkeletons(Skeletons *skeletons) override;

    virtual bool worldToImage(const std::vector<Vec3f> &world, std::vector<Vec2f> &image) const override;
    virtual bool worldToImage(Vec3f world, Vec2f &image) const override;

private:
	cv::Mat _lastFrame;

	// Pointer to the kinect device
	IKinectSensor *_sensor = nullptr;
	
	// Pointer to the reader, that allows to read depth data from kinect
	IDepthFrameReader *_depthFrameReader = nullptr;
	uint16_t *_depthBuffer = nullptr;

	// Pointer to resources needed for reading skeleton from device
	ICoordinateMapper *_coordMapper = nullptr;
	IBodyFrameReader *_bodyFrameReader = nullptr;
};

inline
KinectCamera::~KinectCamera()
{
	
}

}