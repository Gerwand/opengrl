#pragma once

#include <grl/gesture/HandSkeletonExtractor.h>
#include <grl/classification/Classificator.h>

namespace grl {

class GestureClassificator
{
public:
    struct GestureMatchDescriptor
    {
        const std::string *gestureCategory;
        float score1;
        size_t score2;
    };

    virtual GestureMatchDescriptor recognize(const HandSkeleton &skeleton) const = 0;
};

class BoneOrientationClassificator : public GestureClassificator
{
public:
    static constexpr size_t grlHandBonesNumber = 20;

private:
    using KNN = KNNClassificator<HandSkeleton, Vec3f, grlHandBonesNumber>;

public:
    using Features = VectorOfFeatures<Vec3f, grlHandBonesNumber>;
    using SkeletonWithFeature = typename KNN::ObjectWithFeatures;

    void init(size_t knn, float certaintyThreshold);

    GestureMatchDescriptor recognize(const HandSkeleton &skeleton) const override;

    void updateDatabase(const std::string &name, const HandSkeleton &gesture);
    void removeGesture(const std::string &name);

    void extractFeatures(const HandSkeleton &gesture, Features &features) const;
private:
    enum HandBoneType {
        grlHandBoneUnknown = -1,
        // Thumb
        grlHandBoneThumbCon = 0,
        grlHandBoneThumbBase,
        grlHandBoneThumbMid,
        grlHandBoneThumbTop,
        // Index finger
        grlHandBoneIndexCon,
        grlHandBoneIndexBase,
        grlHandBoneIndexMid,
        grlHandBoneIndexTop,
        // Middle finger
        grlHandBoneMiddleCon,
        grlHandBoneMiddleBase,
        grlHandBoneMiddleMid,
        grlHandBoneMiddleTop,
        // Ring finger
        grlHandBoneRingCon,
        grlHandBoneRingBase,
        grlHandBoneRingMid,
        grlHandBoneRingTop,
        // Pinky finger
        grlHandBonePinkyCon,
        grlHandBonePinkyBase,
        grlHandBonePinkyMid,
        grlHandBonePinkyTop,

        grlHandBoneNum
    };

    struct HandBone {
        HandBoneType type = grlHandBoneUnknown;
        OrientedTransformation vector;
    };

    // Hand without fingers has around 9cm in height
    static constexpr float grlHandMidDepth = 0.09f;

    // first - base
    // second - end
    static constexpr std::array< std::pair<HandJointType, HandJointType>, grlHandJointNum> _boneJointMap = {
        std::make_pair(grlHandJointWrist,       grlHandJointThumbBase), // grlHandBoneThumbCon
        std::make_pair(grlHandJointThumbBase,   grlHandJointThumbMid),  // grlHandBoneThumbBase
        std::make_pair(grlHandJointThumbMid,    grlHandJointThumbTop),  // grlHandBoneThumbMid
        std::make_pair(grlHandJointThumbTop,    grlHandJointThumbTip),  // grlHandBoneThumbTop
        std::make_pair(grlHandJointWrist,       grlHandJointIndexBase), // grlHandBoneIndexCon
        std::make_pair(grlHandJointIndexBase,   grlHandJointIndexMid),  // grlHandBoneIndexBase
        std::make_pair(grlHandJointIndexMid,    grlHandJointIndexTop),  // grlHandBoneIndexMid
        std::make_pair(grlHandJointIndexTop,    grlHandJointIndexTip),  // grlHandBoneIndexTop
        std::make_pair(grlHandJointWrist,       grlHandJointMiddleBase), // grlHandBoneMiddleCon
        std::make_pair(grlHandJointMiddleBase,  grlHandJointMiddleMid), // grlHandBoneMiddleBase
        std::make_pair(grlHandJointMiddleMid,   grlHandJointMiddleTop), // grlHandBoneMiddleMid
        std::make_pair(grlHandJointMiddleTop,   grlHandJointMiddleTip), // grlHandBoneMiddleTop
        std::make_pair(grlHandJointWrist,       grlHandJointRingBase),  // grlHandBoneRingCon
        std::make_pair(grlHandJointRingBase,    grlHandJointRingMid),   // grlHandBoneRingBase
        std::make_pair(grlHandJointRingMid,     grlHandJointRingTop),   // grlHandBoneRingMid
        std::make_pair(grlHandJointRingTop,     grlHandJointRingTip),   // grlHandBoneRingTop
        std::make_pair(grlHandJointWrist,       grlHandJointPinkyBase), // grlHandBonePinkyCon
        std::make_pair(grlHandJointPinkyBase,   grlHandJointPinkyMid),  // grlHandBonePinkyBase
        std::make_pair(grlHandJointPinkyMid,    grlHandJointPinkyTop),  // grlHandBonePinkyMid
        std::make_pair(grlHandJointPinkyTop,    grlHandJointPinkyTip),  // grlHandBonePinkyTop
    };

    KNN _classificator;
    size_t _knn;
    float _certaintyThreshold;

    void generateBones(const HandSkeleton &skeleton, std::vector<HandBone> &bones) const;
};

}
