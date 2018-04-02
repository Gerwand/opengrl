#include "SkeletonExtractor.h"

namespace grl {

void
SkeletonExtractor::extractHands(const cv::Mat &depthImage, const Skeleton &skeleton, Object &leftHand, Object &rightHand)
{
	const Joint &jLeftWrist = skeleton.joints[LEFT_WRIST];
	const Joint &jLeftElbow = skeleton.joints[LEFT_ELBOW];
	const Joint &jLeftShoulder = skeleton.joints[LEFT_SHOULDER];
	bool leftValid = jLeftWrist.tracked && jLeftElbow.tracked;// && jLeftShoulder.tracked;

	const Joint &jRightWrist = skeleton.joints[RIGHT_WRIST];
	const Joint &jRightElbow = skeleton.joints[RIGHT_ELBOW];
	const Joint &jRightShoulder = skeleton.joints[RIGHT_SHOULDER];
	bool rightValid = jRightWrist.tracked && jRightElbow.tracked;// && jRightShoulder.tracked;

    leftHand.reset();
    rightHand.reset();

	if (rightValid) {
		_floodFillRight.setup(_config.tolerance, depthImage);
		Vec3f rElbow = Vec3f(jRightElbow.coordDepthImage.x, jRightElbow.coordDepthImage.y, 
							 depthImage.at<uint16_t>(cv::Point(jRightElbow.coordDepthImage.x, jRightElbow.coordDepthImage.y)));
		
		Vec3f rWrist = Vec3f(jRightWrist.coordDepthImage.x, jRightWrist.coordDepthImage.y,
							 depthImage.at<uint16_t>(cv::Point(jRightWrist.coordDepthImage.x, jRightWrist.coordDepthImage.y)));
		//Vec3f rShoulder = Vec3f(jRightShoulder.coordDepthImage.x, jRightShoulder.coordDepthImage.y,
		//						depthImage.at<uint16_t>(cv::Point(jRightShoulder.coordDepthImage.x, jRightShoulder.coordDepthImage.y)));
		Vec3f vrew, vres, vrmid, vrn;
		// Vector: left elbow and wrist
		vrew = Vec3f(rWrist.x - rElbow.x,
					 rWrist.y - rElbow.y,
					 rWrist.z - rElbow.z);
		/*
		// Vector: left elbow and shoulder
		vres = Vec3f(rShoulder.x - rElbow.x,
					 rShoulder.y - rElbow.y,
					 rShoulder.z - rElbow.z);
		// Vector between elbow and shoulder
		vrmid = (vrew + vres)/2.0f;
		// Vector orthogonal to arm

		// If wrist is right of elbow, then do elbow-wrist cross elbow-shoulder for proper plane orientation
		if (rWrist.x > rShoulder.x)
			vrn = vrew.cross(vres);
		else 
			vrn = vres.cross(vrew);

		Plane rplane = Plane::fromVectors(rElbow, vrn, vrmid);
		*/
		Vec3f normew = Vec3f::normalize(vrew);
		Plane rplane;

		Vec3f hookPoint = rElbow + vrew/2.0f;
		rplane.set(normew, hookPoint);

		_floodFillRight.extractObject(Vec2i{(int)hookPoint.x, (int)hookPoint.y}, rightHand, rplane);
		rightHand.accuracy = 1;
	}

	if (leftValid) {
		_floodFillLeft.setup(_config.tolerance, depthImage);
		Vec3f lElbow = Vec3f(jLeftElbow.coordDepthImage.x, jLeftElbow.coordDepthImage.y,
							 depthImage.at<uint16_t>(cv::Point(jLeftElbow.coordDepthImage.x, jLeftElbow.coordDepthImage.y)));
		Vec3f lWrist = Vec3f(jLeftWrist.coordDepthImage.x, jLeftWrist.coordDepthImage.y,
							 depthImage.at<uint16_t>(cv::Point(jLeftWrist.coordDepthImage.x, jLeftWrist.coordDepthImage.y)));
		
		//Vec3f lShoulder = Vec3f(jLeftShoulder.coordDepthImage.x, jLeftShoulder.coordDepthImage.y,
		//						depthImage.at<uint16_t>(cv::Point(jLeftShoulder.coordDepthImage.x, jLeftShoulder.coordDepthImage.y)));	
		Vec3f vlew, vles, vlmid, vln;
		// Vector: left elbow and wrist
		vlew = Vec3f(lWrist.x - lElbow.x,
					 lWrist.y - lElbow.y,
					 lWrist.z - lElbow.z);
		/*
		// Vector: left elbow and shoulder
		vles = Vec3f(lShoulder.x - lElbow.x,
					 lShoulder.y - lElbow.y,
					 lShoulder.z - lElbow.z);
		// Vector between elbow and shoulder
		vlmid = (vlew + vles)/2.0f;
		// Vector orthogonal to arm
		vln = vles.cross(vlew);
		*/

		//Plane lplane = Plane::fromVectors(lElbow, vlmid, vln);

		Vec3f normew = Vec3f::normalize(vlew);
		Plane lplane;
		Vec3f hookPoint = lElbow + vlew/2.0f;
		lplane.set(normew, hookPoint);

		_floodFillLeft.extractObject(Vec2i{(int)hookPoint.x, (int)hookPoint.y}, leftHand, lplane);

		//lplane.set(normew, lElbow);

		// Extract for elbow - if failed, then elbow must overllap with right hand
		//if (_floodFillLeft.extractObject(jLeftElbow.coordDepthImage, leftHand, lplane)) {
			leftHand.accuracy = 1;
		//}
	}
}

}
