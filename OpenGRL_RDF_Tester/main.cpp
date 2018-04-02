#include "ImageWindow.h"
#include "../OpenGRL_GUI/RandomDecisionForest.h"

static grl::GaussianKernel kernelBig(25.0f, 51);
static grl::GaussianKernel kernelSmall(12.0f, 25);

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
    // Pinky finker
    grlHandJointPinkyBase,
    grlHandJointPinkyMid,
    grlHandJointPinkyTop,
    grlHandJointPinkyTip,

    grlHandJointNum
};

constexpr HandJointType handClassToJointMap[] = {
    // Hand
    grlHandJointWrist,
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
    // Pinky finker
    grlHandJointPinkyBase,
    grlHandJointPinkyMid,
    grlHandJointPinkyTop,
    grlHandJointPinkyTip,
};

constexpr HandJointType handClassToHandJoint(HandClass handClass)
{
    return handClassToJointMap[handClass];
}

enum {
    grlHandJointValid = 0,
    grlHandJointInvalid = -1
};

struct HandJoint {
    Vec3f location{ 0.0f, 0.0f, 0.0f };
    float certainty = 0.0f;
    HandJointType type = grlHandJointUnknown;
};

struct Hand2DBone {
    HandJointType boneBase;
    HandJointType boneEnd;
    Vec2f normalVector;
    float length;
    Hand2DBone *parentBone = nullptr;
    Hand2DBone *childrenBone = nullptr;
};

class HandSkeleton
{
public:
    HandSkeleton() {};
    HandSkeleton(const std::vector<HandJoint> &joint);
    void setJoint(HandJoint joint);
    HandJoint & getJoint(HandJointType jointType) { return _joints[jointType]; };
    const HandJoint & getJoint(HandJointType jointType) const { return _joints[jointType]; };
    HandJoint & operator[](size_t i) { return _joints[i]; }
    const HandJoint & operator[](size_t i) const { return _joints[i]; };

    void getAllChildren(HandJointType jointType, std::vector<HandJoint> &joints);
    HandJoint & getParent(HandJointType jointType);
    const HandJoint & getParent(HandJointType jointType) const;
    void generateBones(std::vector<Hand2DBone> &bones);

    void drawOnImage(cv::Mat &image);
private:
    std::array<HandJoint, grlHandJointNum> _joints;

    constexpr static std::array<HandJointType, grlHandJointNum> _jointsParents = {
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

void 
HandSkeleton::generateBones(std::vector<Hand2DBone> &bones)
{
    Vec2i wristLocation;
    // If the location of the wrist is unknown (because it is occluded by the knuckles), it's location is
    // going to be determined using the knuckles location. If it is occluded by the knuckles, they must be visible.
    if (_joints[grlHandJointWrist].certainty <= 0.01f) {
        std::vector<HandJoint> knuckles;
        getAllChildren(grlHandJointWrist, knuckles);
        wristLocation.x = 0;
        wristLocation.y = 0;
        for (auto it = knuckles.begin(); it != knuckles.end(); ++it) {
            // The thumb should be skipped
            if (it->type == grlHandJointThumbBase)
                continue;
            wristLocation.x += it->location.x;
            wristLocation.y += it->location.y;
        }
        wristLocation.x /= knuckles.size();
        wristLocation.y /= knuckles.size();
    } else {
        wristLocation.x = _joints[grlHandJointWrist].location.x;
        wristLocation.y = _joints[grlHandJointWrist].location.y;
    }

    // Get base bones

}

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

inline
HandSkeleton::HandSkeleton(const std::vector<HandJoint> &joint)
{
    for (auto it = joint.cbegin(); it != joint.cend(); ++it) {
        assert(it->type != grlHandJointUnknown);
        _joints[it->type] = *it;
    }
}

inline void
HandSkeleton::setJoint(HandJoint joint)
{
    _joints[joint.type] = joint;
}

inline void
HandSkeleton::getAllChildren(HandJointType jointType, std::vector<HandJoint> &joints)
{
    const std::vector<HandJointType> &types = _jointsChildren[jointType];
    joints.clear();
    for (auto it = types.cbegin(); it != types.cend(); ++it)
        joints.push_back(_joints[*it]);
}

inline HandJoint &
HandSkeleton::getParent(HandJointType jointType)
{
    return _joints[_jointsParents[jointType]];
}

inline const HandJoint &
HandSkeleton::getParent(HandJointType jointType) const
{
    return _joints[_jointsParents[jointType]];
}

inline void
HandSkeleton::drawOnImage(cv::Mat &image)
{
    // Draw lines
    for (auto itj = _joints.cbegin(); itj < _joints.cend(); ++itj) {
        if (itj->certainty <= 0.01f)
            continue;
        std::vector<HandJoint> children;
        getAllChildren(itj->type, children);
        for (auto itc = children.cbegin(); itc != children.cend(); ++itc) {
            if (itc->certainty <= 0.01f)
                continue;
            cv::line(image, 
                     cv::Point(itj->location.x, itj->location.y),
                     cv::Point(itc->location.x, itc->location.y),
                     cv::Scalar(125, 0, 125), 1);
        }
    }

    // Draw points
    for (auto itj = _joints.cbegin(); itj < _joints.cend(); ++itj) {
        if (itj->certainty <= 0.01f)
            continue;
        cv::circle(image, cv::Point(itj->location.x, itj->location.y), 1, cv::Scalar(255, 255, 255), 2);
    }
}

void updateAngles()
{

}

}

static std::array<const grl::GaussianKernel *, grl::grlHandIndexNum> kernels = {
    &kernelBig, //Wrist
    &kernelBig, //Center
    &kernelSmall, //Thumb base
    &kernelSmall, //Thumb mid
    &kernelSmall, //Thumb top
    &kernelSmall, //Thumb tip
    &kernelSmall, //Index base
    &kernelSmall, //Index mid
    &kernelSmall, //Index top
    &kernelSmall, //Index tip
    &kernelSmall, //Middle base
    &kernelSmall, //Middle mid
    &kernelSmall, //Middle top
    &kernelSmall, //Middle tip
    &kernelSmall, //Ring base
    &kernelSmall, //Ring mid
    &kernelSmall, //Ring top
    &kernelSmall, //Ring tip
    &kernelSmall, //Pinky base
    &kernelSmall, //Pinky mid
    &kernelSmall, //Pinky top
    &kernelSmall, //Pinky tip
};

using JointsApproximation = std::array<grl::HandJoint, grl::grlHandIndexNum>;

std::pair<cv::Point2i, float> densityEstimator(const cv::Mat &weights, const grl::GaussianKernel &kernel, cv::Point centerPoint)
{
    int centerOffsets = static_cast<int>(kernel.getSize() / 2);
    int xStart = centerPoint.x - centerOffsets;
    int yStart = centerPoint.y - centerOffsets;
    
    int xEnd = centerPoint.x + centerOffsets;
    int yEnd = centerPoint.y + centerOffsets;
	
    cv::Rect roi(
        grl::clampMin(xStart, 0),
        grl::clampMin(yStart, 0),
        grl::clampMax(xEnd, weights.cols) - grl::clampMin(xStart, 0),
        grl::clampMax(yEnd, weights.rows) - grl::clampMin(yStart, 0));
    const cv::Mat &weightsROI = weights(roi);
    
    int skipLeft = grl::clampMin(-xStart, 0);
    //int skipRight = grl::clampMin(xEnd - weights.cols, 0);
    int skipTop = grl::clampMin(-yStart, 0);
    //int skipBottom = grl::clampMin(yEnd - weights.rows, 0);
    
    float sum = 0.0f;
    float gradientX = 0.0f;
    float gradientY = 0.0f;
    for (int y = 0, ky = skipTop, realY = roi.y; y < weightsROI.rows; ++ky, ++y, ++realY) {
        for (int x = 0, kx = skipLeft, realX = roi.x; x < weightsROI.cols; ++kx, ++x, ++realX) {
            float kval = kernel(kx, ky);
            float wval = weightsROI.at<float>(cv::Point(x, y));
            float value = wval * kval;
            gradientX += value * realX;
            gradientY += value * realY;
            sum += value;
        }
    }

    std::pair<cv::Point2i, float> result =
        sum > 0.0f ?
        std::pair<cv::Point2i, float>(
            cv::Point(static_cast<int>(gradientX / sum), static_cast<int>(gradientY / sum)),
            sum / roi.width / roi.height) :
        std::pair<cv::Point2i, float>(
            cv::Point(centerPoint.x, centerPoint.y),
            0.0f);

    return result;
}

void approximateJoints(const cv::Mat &depthImage,
                       const grl::ClassesWeights &weights,
                       grl::HandSkeleton &skeleton,
                       const grl::ClassesPoints &bestProbabilities)
{
    constexpr float accuracyThresh = 0.8f;

    for (size_t i = 0; i < grl::grlHandIndexNum; ++i) {
        grl::HandJointType jointIndex = grl::handClassToHandJoint(static_cast<grl::HandClass>(i));
        if (jointIndex == grl::grlHandJointUnknown)
            continue;
        const cv::Mat &classWeights = weights[i];
        const grl::GaussianKernel &kernel = *kernels[i];
        const std::list<grl::Vec2i> &startingPoints = bestProbabilities[i];
        int minOffset = static_cast<int>(kernel.getSize() / 2);

        std::pair<cv::Point2i, float> densityResult;

        for (auto it = startingPoints.begin(); it != startingPoints.end(); ++it) {
            cv::Point centerPoint(it->x, it->y);
            cv::Point oldPoint;
            do {
                oldPoint = centerPoint;
                densityResult = densityEstimator(classWeights, kernel, centerPoint);
                centerPoint = densityResult.first;
            } while (oldPoint != centerPoint);

            if (skeleton[jointIndex].certainty < densityResult.second) {
                skeleton[jointIndex].certainty = densityResult.second;
                skeleton[jointIndex].location = grl::Vec3f{ static_cast<float>(densityResult.first.x), 
                                                            static_cast<float>(densityResult.first.y),
                                                            depthImage.at<float>(densityResult.first) };
                skeleton[jointIndex].type = jointIndex;
            }
        }
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const char *className = "H:\\Magisterka\\OpenGRL\\OpenGRL_GestureGenerator\\generated-train-small\\hand_classes_0017701.png";
    const char *depthName = "H:\\Magisterka\\OpenGRL\\OpenGRL_GestureGenerator\\generated-train-small\\hand_depth_0017701.exr";
    cv::Mat classImage, depthImage;
    grl::loadDepthImageWithClasses(className, depthName, classImage, depthImage);

    cv::Mat classImageRGB;
    grl::convertHandClassesToRGB(classImage, classImageRGB);
    ImageWindow imgw1(classImageRGB);

    grl::RandomDecisionForest forest;

    if (!forest.loadFromFile("forest-new.txt"))
        std::cout << "Invalid forest" << std::endl;

    cv::Mat calculatedClasses;
    grl::ClassesWeights weights;
    grl::ClassesPoints bestProbabilities;
    forest.classifyImage(depthImage, calculatedClasses, weights, bestProbabilities);

    cv::Mat calculatedClassesRGB;
    grl::convertHandClassesToRGB(calculatedClasses, calculatedClassesRGB);
    ImageWindow imgw2(calculatedClassesRGB);

    grl::HandSkeleton joints;
    approximateJoints(depthImage, weights, joints, bestProbabilities);
    joints.drawOnImage(calculatedClassesRGB);

    ImageWindow imgw3(calculatedClassesRGB);

    imgw1.show();
    imgw2.show();
    imgw3.show();

    return app.exec();
}
