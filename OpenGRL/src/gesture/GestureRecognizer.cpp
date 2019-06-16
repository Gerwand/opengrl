#include <grl/gesture/GestureRecognizer.h>

#include <grl/utils/ImageToolkit.h>
#include <grl/utils/DebugTools.h>

namespace grl {

bool
GestureRecognizer::init(DepthCamera *camera,
                        GestureExtractor *extractor,
                        GestureTracker *tracker,
                        GestureMatcher *matcher)
{
	if (_valid) {
		DEBUG_ERR("Recognizer already initialized!");
		return false;
	}

	_depthCamera = camera;
	_extractor = extractor;
    _tracker = tracker;
	_matcher = matcher;

	_valid = true;

	return true;
}

RecognitionStatus
GestureRecognizer::update()
{
	cv::Mat depthFrame;
	if (!_depthCamera->getFrame(&depthFrame))
		return NoInput;

	// Enahnce image before editing
	EnhanceDepthFrame(depthFrame);

	// Visualise depth map
	_normalizedDepth = DepthToColor(depthFrame);

	// Get skeletons
	Skeletons skeletons;
	if (!_depthCamera->getSkeletons(&skeletons))
		return NoSkeleton;

	if (skeletons.empty())
		return NoSkeleton;

	// Check if there is skeleton with valid lean

	std::vector<Skeleton *> validSkeletons;
	for (auto it = skeletons.begin(); it != skeletons.end(); ++it)
		if (it->lean <= maxLeanAngle) validSkeletons.push_back(&(*it));

	if (validSkeletons.empty())
		return NoSkeleton;

	Skeleton *closestSkeleton = *validSkeletons.begin();
	for (auto it = validSkeletons.begin() + 1; it != validSkeletons.end(); ++it)
		if (closestSkeleton->distance > (*it)->distance) closestSkeleton = *it;

	_skeleton = *closestSkeleton;

    _tracker->update(_skeleton.joints[LEFT_HAND], _skeleton.joints[RIGHT_HAND]);

	_extractor->extractHands(depthFrame, _skeleton, _leftHand, _rightHand);

	if (_leftHand.getAccuracy() == 0 && _rightHand.getAccuracy() == 0)
		return NoHands;

	// Get objects from current frame
	// _analyzer->extractObjects(depthFrame);

	// _handFound = _analyzer->findHands(_extractedHand);

	// If there was no collisions and any object was found
	// Try to recognize the gesture and write info about object
	// if (_handFound) {
		// Enhance Image
		// EnhanceExtractedHand(_extractedHand);
		
		// Skip matching for now
		// _lastMatch = _matcher->matchBestGesture(_extractedHand);

		// return Recognized;
	// };
	return NoGesture;
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