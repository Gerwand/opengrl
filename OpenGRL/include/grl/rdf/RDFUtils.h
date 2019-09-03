#pragma once

#include <grl/gesture/HandSkeleton.h>

namespace grl {

enum HandClass {
    grlUnknownIndex = -1,
    // BG
    grlBackgroundIndex = -1,
    grlHandIndexOffset = 1,
    grlHandIndexStart = 0,
    // Hand
    grlWristIndex = grlHandIndexStart,
    grlCenterIndex,
    // Thumb
    grlThumbBaseIndex,
    grlThumbMidIndex,
    grlThumbTopIndex,
    grlThumbTipIndex,
    // Index finger
    grlIndexBaseIndex,
    grlIndexMidIndex,
    grlIndexTopIndex,
    grlIndexTipIndex,
    // Middle finger
    grlMiddleBaseIndex,
    grlMiddleMidIndex,
    grlMiddleTopIndex,
    grlMiddleTipIndex,
    // Ring finger
    grlRingBaseIndex,
    grlRingMidIndex,
    grlRingTopIndex,
    grlRingTipIndex,
    // Pinky finger
    grlPinkyBaseIndex,
    grlPinkyMidIndex,
    grlPinkyTopIndex,
    grlPinkyTipIndex,

    grlHandIndexNum,
};

enum HandClassColor {
// BG
    grlBackgroundColor = 0x000000,
    // Hand
    grlWristColor = 0xFF0000,
    grlCenterColor = 0xBC00BC,
    // Thumb
    grlThumbBaseColor = 0xBCFFBC,
    grlThumbMidColor = 0xFFFF00,
    grlThumbTopColor = 0x00FFBC,
    grlThumbTipColor = 0x00FF00,
    // Index finger
    grlIndexBaseColor = 0xBCBCFF,
    grlIndexMidColor = 0xBC00FF,
    grlIndexTopColor = 0x00BCFF,
    grlIndexTipColor = 0x0000FF,
    // Middle finger
    grlMiddleBaseColor = 0x6389BC,
    grlMiddleMidColor = 0x63BC89,
    grlMiddleTopColor = 0x89BCBC,
    grlMiddleTipColor = 0x00FFFF,
    // Ring finger
    grlRingBaseColor = 0x898989,
    grlRingMidColor = 0xA5A5A5,
    grlRingTopColor = 0xBCBCBC,
    grlRingTipColor = 0xE1E1E1,
    // Pinky finger
    grlPinkyBaseColor = 0xBC8963,
    grlPinkyMidColor = 0x89BC63,
    grlPinkyTopColor = 0xBCBC89,
    grlPinkyTipColor = 0xFF00FF,
};

#ifdef USE_GPU
#pragma pack(push, 1)
typedef struct Pixel
{
    Vector2<cl_short> coords;
    cl_float depth;
    cl_uint imgID;
    cl_char classIndex;
} Pixel;
#pragma pack(pop)
#else // !USE_GPU
typedef struct Pixel
{
    Vector2<short> coords;
    float depth;
    uint32_t imgID;
    int8_t classIndex;
} Pixel;
#endif

class RDFTools {
public:
    static constexpr HandJointType handClassToHandJoint(HandClass handClass) {
        return handClassToJointMap[handClass];
    }

    static void convertRGBToHandClasses(const cv::Mat &src, cv::Mat &dst);
    static void convertHandClassesToRGB(const cv::Mat &src, cv::Mat &dst);

    static cv::Rect loadDepthImageWithClasses(
        const std::string &className,
        const std::string &depthName,
        cv::Mat &classImage,
        cv::Mat &depthImage);
    static void loadDepthImagesWithClasses(
        size_t start,
        size_t stop,
        size_t step,
        uint8_t nameDigits,
        const std::string &className,
        const std::string &depthName,
        std::vector<cv::Mat> &classImages,
        std::vector<cv::Mat> &depthImages);

private:
    static void convertBlenderDepthToGeneric(const cv::Mat &src, cv::Mat &dst);

    static constexpr HandJointType handClassToJointMap[] = {
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

    static constexpr std::array<HandClassColor, grlHandIndexNum + grlHandIndexOffset> classColors = {
        grlBackgroundColor,
        grlWristColor, grlCenterColor,
        grlThumbBaseColor, grlThumbMidColor, grlThumbTopColor, grlThumbTipColor,
        grlIndexBaseColor, grlIndexMidColor, grlIndexTopColor, grlIndexTipColor,
        grlMiddleBaseColor, grlMiddleMidColor, grlMiddleTopColor, grlMiddleTipColor,
        grlRingBaseColor, grlRingMidColor, grlRingTopColor, grlRingTipColor,
        grlPinkyBaseColor, grlPinkyMidColor, grlPinkyTopColor, grlPinkyTipColor,
    };
};

}
