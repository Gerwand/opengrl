#include <grl/gesture/HandSkeleton.h>

#include <fstream>

namespace grl {

const std::array<const std::vector<HandJointType>, grlHandJointNum> HandSkeleton::_jointsChildren = {
    // Wrist
    std::vector<HandJointType>{ grlHandJointThumbBase, grlHandJointIndexBase,
                                grlHandJointMiddleBase, grlHandJointRingBase,
                                grlHandJointPinkyBase },
    // Center
    std::vector<HandJointType>{},
    // Thumb
    std::vector<HandJointType>{ grlHandJointThumbMid }, // Thumb base
    std::vector<HandJointType>{ grlHandJointThumbTop }, // Thumb mid
    std::vector<HandJointType>{ grlHandJointThumbTip }, // Thumb top
    std::vector<HandJointType>{},                       // Thumb tip
    // Index finger
    std::vector<HandJointType>{ grlHandJointIndexMid }, // Index base
    std::vector<HandJointType>{ grlHandJointIndexTop }, // Index mid
    std::vector<HandJointType>{ grlHandJointIndexTip }, // Index top
    std::vector<HandJointType>{},                       // Index tip
    // Middle finger
    std::vector<HandJointType>{ grlHandJointMiddleMid }, // Middle base
    std::vector<HandJointType>{ grlHandJointMiddleTop }, // Middle mid
    std::vector<HandJointType>{ grlHandJointMiddleTip }, // Middle top
    std::vector<HandJointType>{},                       // Middle tip
    // Ring finger
    std::vector<HandJointType>{ grlHandJointRingMid }, // Ring base
    std::vector<HandJointType>{ grlHandJointRingTop }, // Ring mid
    std::vector<HandJointType>{ grlHandJointRingTip }, // Ring top
    std::vector<HandJointType>{},                       // Ring tip
    // Pinky finker
    std::vector<HandJointType>{ grlHandJointPinkyMid }, // Pinky base
    std::vector<HandJointType>{ grlHandJointPinkyTop }, // Pinky mid
    std::vector<HandJointType>{ grlHandJointPinkyTip }, // Pinky top
    std::vector<HandJointType>{},                       // Pinky tip
};

HandSkeleton::HandSkeleton()
{
    for (auto it = _joints.begin(); it != _joints.end(); ++it)
        memset(&(*it), 0, sizeof(HandJoint));
}

HandSkeleton::HandSkeleton(const std::vector<HandJoint> &joint)
{
    for (auto it = _joints.begin(); it != _joints.end(); ++it)
        memset(&(*it), 0, sizeof(HandJoint));

    for (auto it = joint.cbegin(); it != joint.cend(); ++it) {
        assert(it->type != grlHandJointUnknown);
        _joints[it->type] = *it;
    }
}

void HandSkeleton::setJoint(HandJoint joint)
{
    _joints[joint.type] = joint;
}

void HandSkeleton::getAllChildren(HandJointType jointType, std::vector<HandJoint *> &joints)
{
    const std::vector<HandJointType> &types = _jointsChildren[jointType];
    joints.clear();
    for (auto it = types.cbegin(); it != types.cend(); ++it)
        joints.push_back(&_joints[*it]);
}

void HandSkeleton::getAllChildren(HandJointType jointType, std::vector<const HandJoint *> &joints) const
{
    const std::vector<HandJointType> &types = _jointsChildren[jointType];
    joints.clear();
    for (auto it = types.cbegin(); it != types.cend(); ++it)
        joints.push_back(&_joints[*it]);
}

HandJoint & HandSkeleton::getParent(HandJointType jointType)
{
    return _joints[_jointsParents[jointType]];
}

const HandJoint & HandSkeleton::getParent(HandJointType jointType) const
{
    return _joints[_jointsParents[jointType]];
}

bool saveHandSkeletonToFile(const HandSkeleton &skeleton,
                            const std::string &fileName)
{
    std::ofstream file(fileName, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open())
        return false;

    // Write down all of the joints
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        const HandJoint &joint = skeleton[i];
        file << joint.location.x << ' '
             << joint.location.y << ' '
             << joint.location.z << ';'
             << joint.certainty << ';'
             << joint.locationImage.x << ' '
             << joint.locationImage.y << ';'
             << joint.type
             << std::endl;
    }
    return true;
}

bool loadHandSkeletonFromFile(HandSkeleton &skeleton,
                              const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        return false;

    // Write down all of the joints
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        HandJoint joint;
        char delim;
        int type;
        file >> joint.location.x >> joint.location.y >> joint.location.z
            >> delim
            >> joint.certainty
            >> delim
            >> joint.locationImage.x >> joint.locationImage.y
            >> delim
            >> type;
        joint.type = static_cast<HandJointType>(type);
        skeleton[i] = joint;
    }
    return true;
}

}
