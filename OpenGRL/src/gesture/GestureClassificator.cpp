#include <grl/gesture/GestureClassificator.h>

namespace grl {

void BoneOrientationClassificator::init(
    size_t knn,
    float certaintyThreshold)
{
    _knn = knn;
    _certaintyThreshold = certaintyThreshold;
}

BoneOrientationClassificator::GestureMatchDescriptor BoneOrientationClassificator::recognize(
    const HandSkeleton &skeleton) const
{
    GestureMatchDescriptor descriptor;

    Features features;
    extractFeatures(skeleton, features);
    typename KNN::KNNDescriptor knnDescriptor = _classificator.classify(features, _knn);

    descriptor.gestureCategory = knnDescriptor.matchName;
    descriptor.score1 = knnDescriptor.matchAverageDistance;
    descriptor.score2 = knnDescriptor.matchNumber;

    return descriptor;
}

void BoneOrientationClassificator::updateDatabase(
    const std::string &name,
    const HandSkeleton &gesture)
{
    auto descriptor = std::make_unique<KNN::ObjectWithFeatures>();
    descriptor->object = gesture;
    extractFeatures(gesture, descriptor->features);

    _classificator.addObjectWithFeatures(name, std::move(descriptor));
}

void BoneOrientationClassificator::removeGesture(const std::string &name)
{
    _classificator.removeClass(name);
}

void BoneOrientationClassificator::extractFeatures(
    const HandSkeleton &gesture,
    Features &features) const
{
    std::vector<HandBone> bones;
    generateBones(gesture, bones);

    // Some orientations may have not visible joints
    std::array<Vec3f, grlHandBoneNum> orientations;
    std::fill(orientations.begin(), orientations.end(), Vec3f(0.0f, 0.0f, 0.0f));

    for (auto it = bones.begin(); it != bones.end(); ++it)
        orientations[it->type] = it->vector.getUnitVector();

    features = Features(orientations);
}


void BoneOrientationClassificator::generateBones(
    const HandSkeleton &skeleton,
    std::vector<BoneOrientationClassificator::HandBone> &bones) const
{
    Vec3f wristLocation;

    // If the location of the wrist is unknown (because it is occluded by the
    // knuckles), it's location is going to be determined using the knuckles
    // location. If it is occluded by the knuckles, they must be visible.
    if (skeleton[grlHandJointWrist].certainty <= _certaintyThreshold) {
        std::vector<const HandJoint *> knuckles;
        skeleton.getAllChildren(grlHandJointWrist, knuckles);
        wristLocation = Vec3f(0.0f, 0.0f, 0.0f);
        for (auto it = knuckles.begin(); it != knuckles.end(); ++it) {
            const HandJoint &joint = **it;
            // The thumb should be skipped
            if (joint.type == grlHandJointThumbBase)
                continue;
            wristLocation += joint.location;
        }
        wristLocation.x /= knuckles.size();
        wristLocation.y /= knuckles.size();
        wristLocation.z /= knuckles.size();
        // Push the wrist back
        wristLocation.z -= grlHandMidDepth;
    } else {
        wristLocation = skeleton[grlHandJointWrist].location;
    }

    // Get bones connecting fingers with wrist
    static constexpr std::array<HandBoneType, 5> conBones = {
        grlHandBoneThumbCon,
        grlHandBoneIndexCon,
        grlHandBoneMiddleCon,
        grlHandBoneRingCon,
        grlHandBonePinkyCon,
    };

    for (auto it = conBones.cbegin(); it != conBones.cend(); ++it) {
        // Get the ending joing of the bone
        HandJointType childJointType = _boneJointMap[*it].second;
        const HandJoint &endingJoint = skeleton[childJointType];

        if (endingJoint.certainty <= _certaintyThreshold)
            continue;

        HandBone bone;
        bone.type = *it;

        Vec3f transformation = endingJoint.location - wristLocation;
        bone.vector = OrientedTransformation(transformation);

        bones.push_back(bone);
    }

    // Rest of the bones without the connection ones
    static constexpr std::array<HandBoneType, grlHandBoneNum - 5> restOfBones = {
        grlHandBoneThumbBase,
        grlHandBoneThumbMid,
        grlHandBoneThumbTop,
        grlHandBoneIndexBase,
        grlHandBoneIndexMid,
        grlHandBoneIndexTop,
        grlHandBoneMiddleBase,
        grlHandBoneMiddleMid,
        grlHandBoneMiddleTop,
        grlHandBoneRingBase,
        grlHandBoneRingMid,
        grlHandBoneRingTop,
        grlHandBonePinkyBase,
        grlHandBonePinkyMid,
        grlHandBonePinkyTop,
    };

    for (auto it = restOfBones.cbegin(); it != restOfBones.cend(); ++it) {
        // Get the ending joing of the bone
        HandJointType parentJointType = _boneJointMap[*it].first;
        HandJointType childJointType = _boneJointMap[*it].second;
        const HandJoint &startingJoint = skeleton[parentJointType];
        const HandJoint &endingJoint = skeleton[childJointType];

        if (startingJoint.certainty <= _certaintyThreshold ||
            endingJoint.certainty <= _certaintyThreshold)
            continue;

        HandBone bone;
        bone.type = *it;

        Vec3f transformation = endingJoint.location - startingJoint.location;
        bone.vector = OrientedTransformation(transformation);

        bones.push_back(bone);
    }
}

}
