#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTime>

#include "ui_MainWindow.h"
#include "TrackSaver.h"
#include "GestureSaver.h"

#include <grl/gesture/GestureRecognizer.h>
#include <grl/gesture/SkeletonExtractor.h>
#include <grl/camera/KinectCamera.h>
#include <grl/track/GestureTracker.h>
#include <grl/gesture/RDFHandSkeletonExtractor.h>
#include <grl/gesture/GestureClassificator.h>

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
    void recordGesture();

    void recordButtonHandler();
    void recordGestureButtonHandler();
    void saveProfileButtonHandler();

    void updateSliderHandler(int value);

private:
	Ui::MainWindowClass _ui;
    TrackSaver *_trackSaver = nullptr;
    GestureSaver *_gestureSaver = nullptr;

	QTimer *_captureTimer = nullptr;

	grl::KinectCamera		*_kinect = nullptr;
	grl::GestureRecognizer	*_recognizer = nullptr;
	grl::SkeletonExtractor	*_extractor = nullptr;
    grl::GestureTracker     *_rightTracker = nullptr;
    grl::DiscretizedTrackClassification<14> *_trackKNN = nullptr;
    grl::RDFHandSkeletonExtractor *_rdfExtractor = nullptr;
    grl::BoneOrientationClassificator *_gestureKNN = nullptr;

    bool _recordingFinished;

    QTime _endTime;
    QTime _lastTime;
    QTime _idleDeadline;

    float _certaintyValue;

    std::vector<cv::Mat> _handImages;
    std::vector<grl::HandSkeleton> _handSkeletons;

    uint64_t _lastStatus;

    void loadSampleTracks();

    void testDepthAndColorize();

    void testDepthRanges();
    void testDepthMedian();

    void testTrackCapturing();
    void testHandExtract();

    void testHandClassify();

    void drawDepthWithSkeleton();
    void all();
};
