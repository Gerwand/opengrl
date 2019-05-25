#include <grl/rdf/SegmentedDepthUtils.h>

namespace grl {

static std::array<HandClassColor, grlHandIndexNum + grlHandIndexOffset> classColors = {
    grlBackgroundColor,
    grlWristColor, grlCenterColor,
    grlThumbBaseColor, grlThumbMidColor, grlThumbTopColor, grlThumbTipColor,
    grlIndexBaseColor, grlIndexMidColor, grlIndexTopColor, grlIndexTipColor,
    grlMiddleBaseColor, grlMiddleMidColor, grlMiddleTopColor, grlMiddleTipColor,
    grlRingBaseColor, grlRingMidColor, grlRingTopColor, grlRingTipColor,
    grlPinkyBaseColor, grlPinkyMidColor, grlPinkyTopColor, grlPinkyTipColor,
};

void
convertFloat3CTo1C(const cv::Mat &src, cv::Mat &dst)
{
    dst = cv::Mat(src.rows, src.cols, CV_32FC1);
    float * dstData = reinterpret_cast<float *>(dst.data);

    cv::MatConstIterator_<cv::Vec3f> it, end;
    for (it = src.begin<cv::Vec3f>(); it != src.end<cv::Vec3f>(); ++it, ++dstData)
        *dstData = (*it)[0] / 10.0f; // Convert from dm to meters
}

void
convertRGBToHandClasses(const cv::Mat &src, cv::Mat &dst)
{
    dst = cv::Mat(src.rows, src.cols, CV_8SC1);
    int8_t *dstData = reinterpret_cast<int8_t *>(dst.data);

    cv::MatConstIterator_<cv::Vec3b> it, end;
    for (it = src.begin<cv::Vec3b>(); it != src.end<cv::Vec3b>(); ++it, ++dstData) {
        int8_t index = 0;
        uint8_t b = (*it)[0];
        uint8_t g = (*it)[1];
        uint8_t r = (*it)[2];
        for (auto itc = classColors.begin(); itc != classColors.end(); ++itc, ++index) {
            if (compareRGBwithVal(*itc, b, g, r))
                break;
        }
        index -= grlHandIndexOffset;
        if (index >= grlHandIndexNum)
            *dstData = grlUnknownIndex;
        else
            *dstData = index;
    }
}

void
convertHandClassesToRGB(const cv::Mat &src, cv::Mat &dst)
{
    dst = cv::Mat(src.rows, src.cols, CV_8UC3);
    cv::Vec3b *dstData = reinterpret_cast<cv::Vec3b *>(dst.data);

    cv::MatConstIterator_<int8_t> it, end;
    for (it = src.begin<int8_t>(); it != src.end<int8_t>(); ++it, ++dstData) {
        int8_t index = *it + grlHandIndexOffset;

        uint32_t rgb = classColors[index];
        uint8_t b, g, r;
        rgb2val(rgb, b, g, r);

        (*dstData)[0] = r;
        (*dstData)[1] = g;
        (*dstData)[2] = b;
    }
}

cv::Rect
getBoundingBoxRGB(const cv::Mat &img)
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

cv::Rect
loadDepthImageWithClasses(const std::string &className, const std::string &depthName,
                          cv::Mat &classImage, cv::Mat &depthImage)
{

    cv::Mat classRGBImage = cv::imread(className);
    // Get ROI
    cv::Rect bb = getBoundingBoxRGB(classRGBImage);

    // Convert ROI
    cv::Mat classRGBImageROI = classRGBImage(bb);
    convertRGBToHandClasses(classRGBImageROI, classImage);

    cv::Mat depth3CImage = cv::imread(depthName, cv::IMREAD_UNCHANGED);

    // Convert ROI
    cv::Mat depth3CImageROI = depth3CImage(bb);

    convertFloat3CTo1C(depth3CImageROI, depthImage);

    return bb;
}

void
loadDepthImagesWithClasses(size_t start, size_t stop, size_t step,
                           uint8_t nameDigits,
                           const std::string &className, const std::string &depthName,
                           std::vector<cv::Mat> &classImages, std::vector<cv::Mat> &depthImages)
{
    classImages.reserve(stop-start);
    depthImages.reserve(stop-start);
    cv::Size sizeMax(0, 0);
    for (size_t i = start; i < stop; i += step) {
        printf("Image %ju\n", i);
        std::ostringstream ssClass;
        std::ostringstream ssDepth;
        ssClass << className << std::setfill('0') << std::setw(nameDigits) << i << ".png";
        ssDepth << depthName << std::setfill('0') << std::setw(nameDigits) << i << ".exr";

        classImages.push_back(cv::Mat());
        depthImages.push_back(cv::Mat());
        cv::Rect bb = loadDepthImageWithClasses(ssClass.str(), ssDepth.str(), classImages.back(), depthImages.back());

        if (bb.width > sizeMax.width) sizeMax.width = bb.width;
        if (bb.height > sizeMax.height) sizeMax.height = bb.height;
    }

    sizeMax.width = static_cast<int>(pow(2, ceil(log(sizeMax.width)/log(2))));
    sizeMax.height = static_cast<int>(pow(2, ceil(log(sizeMax.width)/log(2))));

    std::cout << "New width and height " << sizeMax.width << "," << sizeMax.height << std::endl;

    // Align all images to the common size (for GPU computing)
    for (auto itc = classImages.begin(), itd = depthImages.begin(); itc != classImages.end(); ++itd, ++itc) {
        cv::copyMakeBorder(*itc, *itc, 0, sizeMax.height - itc->rows, 0, sizeMax.width - itc->cols,
                           cv::BORDER_CONSTANT, cv::Scalar(grlBackgroundIndex));
        cv::copyMakeBorder(*itd, *itd, 0, sizeMax.height - itd->rows, 0, sizeMax.width - itd->cols,
                           cv::BORDER_CONSTANT, cv::Scalar(
                           std::numeric_limits<float>::max()
        ));
    }
}

}