#pragma once

#include <grl/utils/RGBTools.h>

namespace grl {

cv::Rect getBoundingBoxRGB(const cv::Mat &img)
{
    cv::Mat rowSum, colSum;
    cv::reduce(img, rowSum, 1, CV_REDUCE_MAX);
    // Find bounding box
    int rmin;
    for (rmin = 0; rmin < rowSum.rows; ++rmin) {
        if (rowSum.data[rmin*3] != 0 ||
            rowSum.data[rmin*3 + 1] != 0 ||
            rowSum.data[rmin*3 + 2] != 0)
            break;
    }
    int rmax;
    for (rmax = rowSum.rows - 1; rmax >= 0; --rmax) {
        if (rowSum.data[rmax*3] != 0 ||
            rowSum.data[rmax*3 + 1] != 0 ||
            rowSum.data[rmax*3 + 2] != 0)
            break;
    }

    cv::reduce(img, colSum, 0, CV_REDUCE_MAX);
    int cmin;
    for (cmin = 0; cmin < colSum.cols; ++cmin) {
        if (colSum.data[cmin*3] != 0 ||
            colSum.data[cmin*3 + 1] != 0 ||
            colSum.data[cmin*3 + 2] != 0)
            break;
    }
    int cmax;
    for (cmax = colSum.cols - 1; cmax >= 0; --cmax) {
        if (colSum.data[cmax*3] != 0 ||
            colSum.data[cmax*3 + 1] != 0 ||
            colSum.data[cmax*3 + 2] != 0)
            break;
    }

    return cv::Rect(cv::Point2i(cmin, rmin), cv::Point2i(cmax, rmax));
}

}
