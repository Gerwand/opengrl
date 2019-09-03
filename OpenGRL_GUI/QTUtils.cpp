#pragma once

#include "QTUtils.h"

#include <sstream>
#include <iomanip>

void drawCountdown(const QTime &now, const QTime &end, cv::Mat &image)
{
    // Convert time to seconds
    float countdown = now.msecsTo(end)/1000.0f;
    // Build string
    std::stringstream s;
    s << std::fixed << std::setprecision(1) << countdown;
    cv::String number(s.str());

    // And put in on the image
    cv::putText(image, number, cv::Point(image.cols/4, image.rows/2),
                CV_FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(255, 0, 0), 4);
}