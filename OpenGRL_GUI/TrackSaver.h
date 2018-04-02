#pragma once

#include "ui_SaveTrackDialog.h"
#include "DepthCamera.h"
#include "GestureTracker.h"

class TrackSaver : public QDialog
{
    Q_OBJECT
public:
    TrackSaver(QWidget *parent, 
               const grl::DepthCamera &camera,
               grl::GestureTracker &tracker,
               const grl::TrackOffsets *leftTrack = nullptr,
               const grl::TrackOffsets *rightTrack = nullptr);

    void setTracks(const grl::TrackOffsets *leftTrack, const grl::TrackOffsets *rightTrack);

public slots:
    virtual int exec() override;

private slots:
    void saveTrackHandler();

private:
    Ui::SaveTrackDialog _ui;

    const grl::TrackOffsets *_leftTrack;
    const grl::TrackOffsets *_rightTrack;
    grl::TrackPoints _leftTrackProcessed;
    grl::TrackPoints _rightTrackProcessed;

    const grl::DepthCamera &_camera;
    grl::GestureTracker &_tracker;
};

inline void 
TrackSaver::setTracks(const grl::TrackOffsets *leftTrack, const grl::TrackOffsets *rightTrack)
{
    _leftTrack = leftTrack;
    _rightTrack = rightTrack;
}
