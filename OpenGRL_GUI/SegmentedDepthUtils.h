#pragma once

#include "MathUtils.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef USE_GPU
#include <CL/cl.hpp>
#endif

#include <array>
#include <limits>
#include <iostream>

namespace grl {

constexpr uint32_t colorRMask = 0xFF0000;
constexpr uint32_t colorRShift = 16;

constexpr uint32_t colorGMask = 0x00FF00;
constexpr uint32_t colorGShift = 8;

constexpr uint32_t colorBMask = 0x0000FF;
constexpr uint32_t colorBShift = 0;

enum HandClass
{
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

enum HandClassColor
{
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
    uint imgID;
    char classIndex;
} Pixel;
#endif

inline bool
compareRGBwithVal(uint32_t rgb, uint8_t b, uint8_t g, uint8_t r)
{
    return (rgb & colorRMask) >> colorRShift == r &&
        (rgb & colorGMask) >> colorGShift == g &&
        (rgb & colorBMask) >> colorBShift == b;
}

inline void
rgb2val(uint32_t rgb, uint8_t &b, uint8_t &g, uint8_t &r)
{
    r = (rgb & colorRMask) >> colorRShift;
    g = (rgb & colorGMask) >> colorGShift;
    b = (rgb & colorBMask) >> colorBShift;
}

void convertFloat3CTo1C(const cv::Mat &src, cv::Mat &dst);
void convertRGBToHandClasses(const cv::Mat &src, cv::Mat &dst);
void convertHandClassesToRGB(const cv::Mat &src, cv::Mat &dst);
cv::Rect loadDepthImageWithClasses(const std::string &className, const std::string &depthName,
                                   cv::Mat &classImage, cv::Mat &depthImage);
void loadDepthImagesWithClasses(size_t start, size_t stop,
                                uint8_t nameDigits,
                                const std::string &className,
                                const std::string &depthName,
                                std::vector<cv::Mat> &classImages,
                                std::vector<cv::Mat> &depthImages);
}
