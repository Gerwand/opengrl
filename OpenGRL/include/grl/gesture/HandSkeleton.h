#pragma once

#include <grl/utils/MathUtils.h>

#include <array>

namespace grl {

enum HandJointType {
    grlHandJointUnknown = -1,
    // Hand base
    grlHandJointWrist = 0,
    grlHandJointCenter,
    // Thumb
    grlHandJointThumbBase,
    grlHandJointThumbMid,
    grlHandJointThumbTop,
    grlHandJointThumbTip,
    // Index finger
    grlHandJointIndexBase,
    grlHandJointIndexMid,
    grlHandJointIndexTop,
    grlHandJointIndexTip,
    // Middle finger
    grlHandJointMiddleBase,
    grlHandJointMiddleMid,
    grlHandJointMiddleTop,
    grlHandJointMiddleTip,
    // Ring finger
    grlHandJointRingBase,
    grlHandJointRingMid,
    grlHandJointRingTop,
    grlHandJointRingTip,
    // Pinky finger
    grlHandJointPinkyBase,
    grlHandJointPinkyMid,
    grlHandJointPinkyTop,
    grlHandJointPinkyTip,

    grlHandJointNum
};

struct HandJoint {
    Vec3f location;
    float certainty;
    Vec2i locationImage;
    HandJointType type = grlHandJointUnknown;
};

class HandSkeleton
{
public:
    HandSkeleton();
    HandSkeleton(const std::vector<HandJoint> &joint);

    void setJoint(HandJoint joint);

    HandJoint & getJoint(HandJointType jointType) { return _joints[jointType]; };
    const HandJoint & getJoint(HandJointType jointType) const { return _joints[jointType]; };

    HandJoint & operator[](size_t i) { return _joints[i]; }
    const HandJoint & operator[](size_t i) const { return _joints[i]; };

    void getAllChildren(HandJointType jointType, std::vector<HandJoint *> &joints);
    void getAllChildren(HandJointType jointType, std::vector<const HandJoint *> &joints) const;

    HandJoint & getParent(HandJointType jointType);
    const HandJoint & getParent(HandJointType jointType) const;

private:
    std::array<HandJoint, grlHandJointNum> _joints;

    static constexpr std::array<HandJointType, grlHandJointNum> _jointsParents = {
        grlHandJointUnknown, // Wrist
        grlHandJointUnknown, // Center
        // Thumb
        grlHandJointWrist, // Thumb base
        grlHandJointThumbBase, // Thumb mid
        grlHandJointThumbMid, // Thumb top
        grlHandJointThumbTop, // Thumb tip
        // Index finger
        grlHandJointWrist, // Index base
        grlHandJointIndexBase, // Index mid
        grlHandJointIndexMid, // Index top
        grlHandJointIndexTop, // Index tip
        // Middle finger
        grlHandJointWrist, // Middle base
        grlHandJointMiddleBase, // Middle mid
        grlHandJointMiddleMid, // Middle top
        grlHandJointMiddleTop, // Middle tip
        // Ring finger
        grlHandJointWrist, // Ring base
        grlHandJointRingBase, // Ring mid
        grlHandJointRingMid, // Ring top
        grlHandJointRingTop, // Ring tip
        // Pinky finker
        grlHandJointWrist, // Pinky base
        grlHandJointPinkyBase, // Pinky mid
        grlHandJointPinkyMid, // Pinky top
        grlHandJointPinkyTop, // Pinky tip
    };

    static const std::array<const std::vector<HandJointType>, grlHandJointNum> _jointsChildren;
};

bool saveHandSkeletonToFile(const HandSkeleton &skeleton,
                            const std::string &fileName);
bool loadHandSkeletonFromFile(HandSkeleton &skeleton,
                              const std::string &fileName);

}