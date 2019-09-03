#pragma once

#include <cstdint>

#include <iostream>

#include <grl/camera/DepthCamera.h>
#include <grl/gesture/GestureExtractor.h>
#include <grl/track/GestureTracker.h>
#include <grl/gesture/HandSkeletonExtractor.h>
#include <grl/track/TrackClassificator.h>
#include <grl/gesture/GestureClassificator.h>
#include <grl/utils/Profiler.h>

namespace grl {

extern Profiler profiler;

// Maximum lean angle for the skeletons
constexpr float maxLeanAngle = 50.0f;

enum RecognitionStatus
{
    GotNothing               = 0x00,
	GotDepth                 = 0x01,
	GotSkeleton              = 0x02,
	GotHands                 = 0x04,
    GotFinishedTrack         = 0x08,
	GotTrackClassification   = 0x10,
	GotGestureClassification = 0x20,
    GotGesture               = 0x40
};

enum RecognitionOptions
{
    grlDepth                 = 0x01,
    grlTrack                 = 0x02,
    grlTrackClassification   = 0x04,
    grlHandExtraction        = 0x08,
    grlGesture               = 0x10,
    grlGestureClassification = 0x20,
    grlSkeleton              = 0x40,
};

enum RecognitionMode
{
    OnlyDepth        = grlDepth,
    GetSkeleton      = grlDepth | grlSkeleton,
    CaptureTrack     = GetSkeleton | grlTrack,
    ClassifyTrack    = CaptureTrack | grlTrackClassification,
    ExtractHands     = GetSkeleton | grlHandExtraction,
    CaptureGesture   = ExtractHands | grlGesture,
    ClassifyGesture  = CaptureGesture | grlGestureClassification,
    NoClassification = CaptureTrack | CaptureGesture,
    All              = ClassifyTrack | ClassifyGesture,
};

class GestureRecognizer
{
public:
	bool init(DepthCamera *camera,
              GestureExtractor *extractor,
              GestureTracker *rightTracker, // Single hand for now
              TrackClassificator *trackClassificator,
              HandSkeletonExtractor *handSkeletonExtractor,
              GestureClassificator *gestureClassificator);

	uint64_t update(RecognitionMode mode = All);

    bool isValid() { return _valid; }
	void destroy();

    DepthCamera * getCamera() { return _depthCamera; }
	GestureExtractor * getExtractor() { return _extractor; }
    GestureTracker * getRightTracker() { return _rightTracker; }
	TrackClassificator * getTrackClassificator() { return _trackClassificator; }
    HandSkeletonExtractor * getHandSkeletonExtractor() { return _handSkeletonExtractor; }

    const DepthObject & getRightHand() { return _rightHand; }
    const DepthObject & getLeftHand() { return _leftHand; }
	const cv::Mat & getNormalizedDepth() { return _normalizedDepth; }
	const cv::Mat & getExtractedHand() { return _extractedHand; }
	void getHandsImage(cv::Mat &destination);
	const Skeleton & getSkeleton() { return _skeleton; }
    const TrackClassificator::TrackMatchDescriptor & getTrackMatch() { return _trackDesc; }
    const HandSkeleton & getHandSkeleton() { return _rightHandSkeleton; }
    const GestureClassificator::GestureMatchDescriptor & getGestureMatch() { return _gestureDesc; }

    GestureTracker::UpdateState getLastTrackerState() const { return _lastTrackerState; };

private:
	// Data, which can be received by the upper layer for presentation
	cv::Mat _normalizedDepth;
	cv::Mat _extractedHand;

	DepthCamera *_depthCamera;
	GestureExtractor *_extractor;
    GestureTracker *_rightTracker;
    TrackClassificator *_trackClassificator;
    HandSkeletonExtractor *_handSkeletonExtractor;
    GestureClassificator *_gestureClassificator;

	Skeleton _skeleton;
    HandSkeleton _rightHandSkeleton;

	DepthObject _leftHand;
	DepthObject _rightHand;

    TrackClassificator::TrackMatchDescriptor _trackDesc;
    GestureClassificator::GestureMatchDescriptor _gestureDesc;

	bool _valid = false;
	bool _handFound = false;

    GestureTracker::UpdateState _lastTrackerState;
};

}