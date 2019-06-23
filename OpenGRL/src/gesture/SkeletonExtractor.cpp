#include <grl/gesture/SkeletonExtractor.h>

namespace grl {

bool SkeletonExtractor::isHandValid(Side side, const cv::Mat &depthImage, const Skeleton &skeleton) const
{
    const Joint &wrist = skeleton.joints[side == Side::Right ? RIGHT_WRIST : LEFT_WRIST];
    const Joint &elbow = skeleton.joints[side == Side::Right ? RIGHT_ELBOW : LEFT_ELBOW];

    // Both elbow and the wrist must be tracked to be able to extract the arm
    bool isValid = wrist.tracked && elbow.tracked;

    return isValid;
}

void SkeletonExtractor::extractHand(Side side,
                                    const cv::Mat &depthImage,
                                    const Skeleton &skeleton,
                                    DepthObject &hand)
{
    _ff.init(_config.depthTolerance, depthImage);

    // Get elbow point in the 3D space
    const Joint &jElbow = skeleton.joints[side == Side::Right ? RIGHT_ELBOW : LEFT_ELBOW];
    Vec2i elbow2D = jElbow.coordDepthImage;
    Vec3f elbow3D = Vec3f(
        static_cast<float>(elbow2D.x),
        static_cast<float>(elbow2D.y),
        depthImage.at<uint16_t>(static_cast<cv::Point>(elbow2D)));

    // Get wrist point in the 3D space
    const Joint &jWrist = skeleton.joints[side == Side::Right ? RIGHT_WRIST : LEFT_WRIST];
    Vec2i wrist2D = jWrist.coordDepthImage;
    Vec3f wrist3D = Vec3f(
        static_cast<float>(wrist2D.x),
        static_cast<float>(wrist2D.y),
        depthImage.at<uint16_t>(static_cast<cv::Point>(wrist2D)));

    // Vector indicating orientation of the arm
    Vec3f armVector = wrist3D - elbow3D;
    Vec3f armOrientation = Vec3f::normalize(armVector);

    // Plane should be placed in the middle between the wrist and the elbow
    Vec3f hookPoint = elbow3D + armVector/2.0f;

    // Create plane which will be extracting points being in front of it
    Plane plane(armOrientation, hookPoint);

    // Try to extract the object from the image
    if (_ff.extractObject(Vec2i((int)hookPoint.x, (int)hookPoint.y), plane, hand))
        // Set accuracy to 255 to indicate that the object was extracted, as we
        // do not have any algorithm to check accuraccy other way than binary.
        hand.setAccuracy(UINT8_MAX);
}

}
