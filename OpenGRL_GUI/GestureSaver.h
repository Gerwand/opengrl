#pragma once

#include "ui_SaveGestureDialog.h"
#include <grl/gesture/HandSkeleton.h>

class GestureSaver : public QDialog
{
    Q_OBJECT
public:
    GestureSaver(QWidget *parent);

    void setGestures(const std::vector<cv::Mat> *handClasses,
                     const std::vector<grl::HandSkeleton> *hands);

public slots:
    virtual int exec() override;

private slots:
    void saveGestureHandler();

private:
    Ui::SaveGestureDialog _ui;
    const std::vector<cv::Mat> *_handClasses;
    const std::vector<grl::HandSkeleton> *_hands;
    std::map<std::string, size_t> _poseCount;
};

inline void
GestureSaver::setGestures(const std::vector<cv::Mat> *handClasses,
                          const std::vector<grl::HandSkeleton> *hands)
{
    _handClasses = handClasses;
    _hands = hands;
}
