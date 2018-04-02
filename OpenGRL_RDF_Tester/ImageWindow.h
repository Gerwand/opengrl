#pragma once

#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../OpenGRL_GUI/QTUtils.h"

class ImageWindow : public QWidget
{
public:
    ImageWindow(const cv::Mat &image);
private:
    QLabel *_img;
    QHBoxLayout *_layout;
};

