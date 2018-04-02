#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTime>

#include "ui_MainWindow.h"
#include "TrackSaver.h"

#include "GestureRecognizer.h"
#include "SkeletonExtractor.h"
#include "KinectCamera.h"
#include "ORBMatcher.h"
#include "DiscretizedGestureTracker.h"

constexpr int recordDelay = 5;
constexpr int idleDelay = 3;
constexpr int maxIdleFrames = 25;

#define SAFE_QT_NEW(dest, obj)				\
	do {									\
		(dest) = new obj;					\
		if ((dest) == nullptr) {			\
			QMessageBox::critical(this, "Fatal error", "Not enough memory resources to allocate objects"); \
			QCoreApplication::exit(ENOMEM);	\
		}									\
	} while (0)

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private slots:
	void updateVideoContext();
    void recordTrack();

    void recordButtonHandler();
    void saveTrackButtonHandler();
    void exrButtonHandler();

private:
	Ui::MainWindowClass _ui;
    TrackSaver *_trackSaver;

	QTimer *_captureTimer = nullptr;
    
	grl::KinectCamera		*_kinect = nullptr;
	grl::ORBMatcher			*_orbMatcher = nullptr;
	grl::GestureRecognizer	*_recognizer = nullptr;
	grl::SkeletonExtractor	*_extractor = nullptr;
    grl::DiscretizedGestureTracker *_discreteTracker = nullptr;

    grl::TrackRecorder      *_leftRecorder = nullptr;
    grl::TrackRecorder      *_rightRecorder = nullptr;
    bool _recordingLeftFinished;
    bool _recordingRightFinished;

    QTime _endTime;
    QTime _lastTime;
    QTime _idleDeadline;

    grl::OfflineGestureDescriptor _leftTrackDescriptor;
    grl::OfflineGestureDescriptor _rightTrackDescriptor;

    void drawTrack(const std::vector<grl::Vec3f> &track, cv::Scalar color, cv::Mat &image);
    void loadSampleTracks();
    static void drawSimpleSkeleton(const grl::Skeleton &skeleton, cv::Mat &image);
};
