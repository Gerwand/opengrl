#include <grl/gesture/GestureRecognizer.h>

#include <grl/utils/ImageToolkit.h>
#include <grl/utils/DebugTools.h>

namespace grl {

// The object that can be used for profiling if PROFILE is defined
Profiler profiler;

bool GestureRecognizer::init(DepthCamera *camera,
                             GestureExtractor *extractor,
                             GestureTracker *rightTracker,
                             TrackClassificator *trackClassificator,
                             HandSkeletonExtractor *handSkeletonExtractor,
                             GestureClassificator *gestureClassificator)
{
	if (_valid) {
		DEBUG_ERR("Recognizer already initialized!");
		return false;
	}

	_depthCamera = camera;
	_extractor = extractor;
    _rightTracker = rightTracker;
    _trackClassificator = trackClassificator;
    _handSkeletonExtractor = handSkeletonExtractor;
    _gestureClassificator = gestureClassificator;

	_valid = true;

	return true;
}

uint64_t
GestureRecognizer::update(RecognitionMode mode)
{
    uint64_t ret = GotNothing;
    cv::Mat depthFrame;

    // Depth frame extraction
    if (mode & grlDepth) {
        TimeInterval t;
	    if (!_depthCamera->getFrame(&depthFrame))
		    return ret;
        ret |= GotDepth;
        // Enahnce image before editing
        EnhanceDepthFrame(depthFrame);

        t.finish();
        profiler.addTime("depth", t);

        // Visualise depth map
        _normalizedDepth = DepthToColor(depthFrame, 1000, 2000);
    }

    // Skeleton extraction
    if (mode & grlSkeleton) {
        // Get skeletons
        Skeletons skeletons;
        if (!_depthCamera->getSkeletons(&skeletons))
            return ret;

        if (skeletons.empty())
            return ret;

        // Check if there is skeleton with valid lean
        std::vector<Skeleton *> validSkeletons;
        for (auto it = skeletons.begin(); it != skeletons.end(); ++it)
            if (it->lean <= maxLeanAngle) validSkeletons.push_back(&(*it));

        if (validSkeletons.empty())
            return ret;
        ret |= GotSkeleton;

        Skeleton *closestSkeleton = *validSkeletons.begin();
        for (auto it = validSkeletons.begin() + 1; it != validSkeletons.end(); ++it)
            if (closestSkeleton->distance > (*it)->distance) closestSkeleton = *it;

        _skeleton = *closestSkeleton;
    }

    // Track update
    if (mode & grlTrack) {
        TimeInterval t;
        _lastTrackerState = _rightTracker->update(_skeleton.joints[RIGHT_HAND]);
        t.finish();
        profiler.addTime("track", t);
        if (_lastTrackerState == GestureTracker::grlTrackerReset)
            ret |= GotFinishedTrack;
    }

    // Track classification
    if (mode & grlTrackClassification) {
        if (_lastTrackerState == GestureTracker::grlTrackerReset) {
            TimeInterval t;
            _trackDesc = _trackClassificator->recognize(_rightTracker->getLastTrack());
            t.finish();
            profiler.addTime("track classify", t);
        }
    }

    // Hand extraction
    if (mode & grlHandExtraction) {
        TimeInterval t;
	    _extractor->extractHands(depthFrame, _skeleton, _leftHand, _rightHand);
        t.finish();
        profiler.addTime("hand extract", t);
    	if (_leftHand.getAccuracy() > 0 || _rightHand.getAccuracy() > 0)
		    ret |= GotHands;
    }

    // Gesture extraction
    if (mode & grlGesture) {
        TimeInterval t;
        _handSkeletonExtractor->extractSkeleton(_rightHand, _rightHandSkeleton);
        t.finish();
        profiler.addTime("skeleton extract", t);
        ret |= GotGesture;
    }

    // Gesture classification
    if (mode & grlGestureClassification) {
        TimeInterval t;
        _gestureDesc = _gestureClassificator->recognize(_rightHandSkeleton);
        t.finish();
        profiler.addTime("gesture recognize", t);
        ret |= GotGestureClassification;
    }

	return ret;
}

void
GestureRecognizer::getHandsImage(cv::Mat &destination)
{
	int rows, cols;
	_depthCamera->getSize(cols, rows);
	cv::Mat tmp = cv::Mat::zeros(rows, cols, CV_16UC1);
	destination = cv::Mat::zeros(rows, cols, CV_16UC1);

	if (_leftHand.getAccuracy() > 0)
		_leftHand.getDepthImageOfObject().copyTo(tmp(_leftHand.getBoundingBox()));
	if (_rightHand.getAccuracy() > 0)
		_rightHand.getDepthImageOfObject().copyTo(destination(_rightHand.getBoundingBox()));

	cv::max(tmp, destination, destination);

	cv::normalize(destination, destination, 0, UINT8_MAX, cv::NORM_MINMAX, CV_8UC1);

	destination = UINT8_MAX - destination;
}


void
GestureRecognizer::destroy()
{
	if (!_valid) {
		DEBUG_ERR("Device already destroyed!");
		return;
	}

	_valid = false;
	_depthCamera->destroy();
}

}