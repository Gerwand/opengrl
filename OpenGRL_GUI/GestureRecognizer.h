#pragma once

#include <cstdint>

#include <iostream>

#include "DepthCamera.h"
#include "GestureExtractor.h"
#include "GestureTracker.h"
#include "GestureMatcher.h"

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
	
    const Object& getLeftHand() { return _leftHand; }
    const Object& getRightHand() { return _rightHand; }
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

	Object _leftHand;
	Object _rightHand;

	bool _valid = false;
	bool _handFound = false;
};

}