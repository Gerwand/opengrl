#include <grl/utils/ImageToolkit.h>

namespace grl {

void drawSimpleSkeleton(const grl::Skeleton &skeleton, cv::Mat &image)
{
    // Left hand
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_HAND].coordDepthImage.x, skeleton.joints[grl::LEFT_HAND].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_WRIST].coordDepthImage.x, skeleton.joints[grl::LEFT_WRIST].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_WRIST].coordDepthImage.x, skeleton.joints[grl::LEFT_WRIST].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.x, skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.x, skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    // Right hand
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_HAND].coordDepthImage.x, skeleton.joints[grl::RIGHT_HAND].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.x, skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.x, skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.x, skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.x, skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    // Spine
    cv::line(image,
             cv::Point(skeleton.joints[grl::SPINE_BASE].coordDepthImage.x, skeleton.joints[grl::SPINE_BASE].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));

    for (int i = 0; i < grl::JointType::COUNT; ++i) {
        cv::Scalar color = skeleton.joints[i].tracked ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 0, 0);
        cv::circle(image,
                   cv::Point(skeleton.joints[i].coordDepthImage.x, skeleton.joints[i].coordDepthImage.y),
                   5, color);
    }
}

void drawTrack(const TrackPoints &track, cv::Mat &image, const DepthCamera &camera)
{
    if (track.getPointsCount() < 2)
        return;

    std::vector<grl::Vec2f> track2D;
    camera.worldToImage(track.getVectorOfElements(), track2D);
    for (auto it = track2D.cbegin() + 1, itBefore = track2D.cbegin(); it != track2D.cend(); itBefore = it++)
            cv::line(image, cv::Point2f(itBefore->x, itBefore->y), cv::Point2f(it->x, it->y), cv::Scalar(0, 255, 0), 2);

    for (auto it = track2D.cbegin(); it != track2D.cend(); it++)
        cv::circle(image, cv::Point2f(it->x, it->y), 3, cv::Scalar(0.0f, 0.0f, 255.0f));
}

void drawJointLines(const HandSkeleton &skeleton, cv::Mat &image, float certainty)
{
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        const HandJoint &joint = skeleton[i];

        if (joint.certainty <= certainty)
            continue;

        std::vector<const HandJoint *> children;
        skeleton.getAllChildren(joint.type, children);

        for (auto itc = children.cbegin(); itc != children.cend(); ++itc) {
            const HandJoint &jointChild = **itc;
            if (jointChild.certainty <= certainty)
                continue;

            cv::line(image,
                     cv::Point(joint.locationImage.x, joint.locationImage.y),
                     cv::Point(jointChild.locationImage.x, jointChild.locationImage.y),
                     cv::Scalar(125, 0, 125), 1);
        }
    }
}

void drawJointPoints(const HandSkeleton &skeleton, cv::Mat &image, float certainty)
{
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        const HandJoint &joint = skeleton[i];

        if (joint.certainty <= certainty)
            continue;
        cv::circle(image, cv::Point(joint.locationImage.x, joint.locationImage.y), 1, cv::Scalar(255, 255, 255), 2);
    }
}

void drawJointsOnImage(const HandSkeleton &skeleton, cv::Mat &image, float certainty)
{
    drawJointLines(skeleton, image, certainty);
    drawJointPoints(skeleton, image, certainty);
}

}