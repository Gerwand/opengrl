#include "ImageWindow.h"


ImageWindow::ImageWindow(const cv::Mat &image)
{
    _layout = new QHBoxLayout(this);

    _img = new QLabel("Invalid image format", this);
    _layout->addWidget(_img);

    QImage qimg;
    if (image.channels() == 3) {
        cvMatRGBToQImage(image, qimg);
    }
    else if (image.channels() == 1) {
        cvMatGrayToQImage(image, qimg);
    }
    else {
        return;
    }
     
    _img->setPixmap(QPixmap::fromImage(qimg));

    setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    setLayout(_layout);
}