#pragma once

#include <QTime>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

inline void cvMatRGBToQImage(const cv::Mat &cvImage, QImage &qImage)
{
    qImage = QImage((const unsigned char*)cvImage.data,
                    static_cast<int>(cvImage.cols), static_cast<int>(cvImage.rows),
                    static_cast<int>(cvImage.step), QImage::Format_RGB888);
}

inline void cvMatRGBaToQImage(const cv::Mat &cvImage, QImage &qImage)
{
    qImage = QImage((const unsigned char*)cvImage.data,
                    static_cast<int>(cvImage.cols), static_cast<int>(cvImage.rows),
                    static_cast<int>(cvImage.step), QImage::Format_RGBA8888);
}

inline void cvMatGrayToQImage(const cv::Mat &cvImage, QImage &qImage)
{
    qImage = QImage((const unsigned char*)cvImage.data,
                    static_cast<int>(cvImage.cols), static_cast<int>(cvImage.rows),
                    static_cast<int>(cvImage.step), QImage::Format_Grayscale8);
}

void drawCountdown(const QTime &now, const QTime &end, cv::Mat &image);
