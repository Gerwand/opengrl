#pragma once

#include <cstdint>

#include <iostream>

#include <grl/camera/DepthCamera.h>
#include <grl/gesture/GestureExtractor.h>
#include <grl/track/GestureTracker.h>
#include <grl/gesture/GestureMatcher.h>

namespace grl {

// Maximum lean angle for the skeletons
constexpr float maxLeanAngle = 50.0f;

enum RecognitionStatus
{
	NoInput,
	NoSkeleton,
	NoHands,
	NoGesture,
	Recognized,
};

class GestureRecognizer
{
public:
	bool init(DepthCamera *camera, GestureExtractor *extractor, GestureTracker *tracker, GestureMatcher *matcher);
	RecognitionStatus update();
	bool isValid() { return _valid; }
	void destroy();

    DepthCamera * getCamera() { return _depthCamera; }
	GestureExtractor * getExtractor() { return _extractor; }
    GestureTracker * getTracker() { return _tracker; }
	GestureMatcher * getMatcher() { return _matcher; }

    const DepthObject& getLeftHand() { return _leftHand; }
    const DepthObject& getRightHand() { return _rightHand; }
	const cv::Mat & getNormalizedDepth() { return _normalizedDepth; }
	const cv::Mat & getExtractedHand() { return _extractedHand; }
	void getHandsImage(cv::Mat &destination);
	GestureMatcher::Match getLastMatch() { return _lastMatch; }
	const Skeleton& getSkeleton() { return _skeleton; }

private:
	// Data, which can be received by the upper layer for presentation
	cv::Mat _normalizedDepth;
	cv::Mat _extractedHand;
	GestureMatcher::Match _lastMatch;

	DepthCamera *_depthCamera;
	GestureExtractor *_extractor;
    GestureTracker *_tracker;
	GestureMatcher *_matcher;

	Skeleton _skeleton;

	DepthObject _leftHand;
	DepthObject _rightHand;

	bool _valid = false;
	bool _handFound = false;
};

}