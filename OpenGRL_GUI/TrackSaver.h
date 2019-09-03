#pragma once

#include "ui_SaveTrackDialog.h"
#include <grl/camera/DepthCamera.h>
#include <grl/track/GestureTracker.h>

class TrackSaver : public QDialog
{
    Q_OBJECT
public:
    TrackSaver(QWidget *parent, const grl::DepthCamera &camera);

    void setTrack(const grl::TrackPoints *track);

public slots:
    virtual int exec() override;

private slots:
    void saveTrackHandler();

private:
    Ui::SaveTrackDialog _ui;
    const grl::DepthCamera &_camera;
    const grl::TrackPoints *_track;
    cv::Mat _trackImage;
    std::map<std::string, size_t> _poseCount;
};

inline void
TrackSaver::setTrack(const grl::TrackPoints *track)
{
    _track = track;
}
