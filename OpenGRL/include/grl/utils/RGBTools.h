#pragma once

#include <opencv2/core/core.hpp>

#include <stdint.h>

namespace grl {

constexpr uint32_t colorRMask = 0xFF0000;
constexpr uint32_t colorRShift = 16;

constexpr uint32_t colorGMask = 0x00FF00;
constexpr uint32_t colorGShift = 8;

constexpr uint32_t colorBMask = 0x0000FF;
constexpr uint32_t colorBShift = 0;

cv::Rect getBoundingBoxRGB(const cv::Mat &img);

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


}
