#pragma once

#define _USE_MATH_DEFINES

#include <KinectSDK20/Kinect.h>

#include <grl/camera/DepthCamera.h>
#include <grl/utils/MathUtils.h>
#include <grl/utils/DebugTools.h>

namespace grl {

class KinectCamera : public DepthCamera
{
public:
	~KinectCamera();

	virtual int init() override;
	virtual void destroy() override;

	virtual bool getFrame(cv::Mat *image) override;
	virtual bool getSkeletons(Skeletons *skeletons) override;

    const cv::Mat & getLastColor() { return _lastColorFrame; }

    virtual bool worldToImage(const std::vector<Vec3f> &world, std::vector<Vec2f> &image) const override;
    virtual bool worldToImage(Vec3f world, Vec2f &image) const override;
    virtual bool imageToWorld(const std::vector<Vec2f> &image, const std::vector<uint16_t> &depths, std::vector<Vec3f> &world) const;
    virtual bool imageToWorld(Vec3f &world, uint16_t depth, Vec2f image) const;

    void dumpStats();

    void colorizeDepth(cv::Mat &depthWithColor);

private:
	cv::Mat _lastFrame;
    cv::Mat _lastColorFrame;

	// Pointer to the kinect device
	IKinectSensor *_sensor = nullptr;

	// Pointer to the reader, that allows to read depth data from kinect
	IDepthFrameReader *_depthFrameReader = nullptr;
	uint16_t *_depthBuffer = nullptr;

	// Pointer to resources needed for reading skeleton from device
	ICoordinateMapper *_coordMapper = nullptr;
	IBodyFrameReader *_bodyFrameReader = nullptr;

    // Color frame
    IColorFrameReader *_colorFrameReader = nullptr;

    bool updateColorFrame();
};

inline
KinectCamera::~KinectCamera()
{

}

}