#pragma once

inline void cvMatRGBToQImage(const cv::Mat &cvImage, QImage &qImage)
{
    qImage = QImage((const unsigned char*)cvImage.data, 
                    static_cast<int>(cvImage.cols), static_cast<int>(cvImage.rows),
                    static_cast<int>(cvImage.step), QImage::Format_RGB888);
}

inline void cvMatGrayToQImage(const cv::Mat &cvImage, QImage &qImage)
{
    qImage = QImage((const unsigned char*)cvImage.data,
                    static_cast<int>(cvImage.cols), static_cast<int>(cvImage.rows),
                    static_cast<int>(cvImage.step), QImage::Format_Grayscale8);
}
