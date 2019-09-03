#include "MainWindow.h"

#include <grl/rdf/RandomDecisionForest.h>

// Temporary solution TODO: load database from file
#include "../OpenGRL_RDF_Tester/SampleTracks.h"
#include "QTUtils.h"

#include <QTimer>
#include <QFileDialog>

#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	int rc;

	SAFE_QT_NEW(_kinect, grl::KinectCamera);
	rc = _kinect->init();
	if (rc != 0) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing kinect device");
		QCoreApplication::exit(rc);
	}

	SAFE_QT_NEW(_orbMatcher, grl::ORBMatcher);
	if (!_orbMatcher->init()) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing matcher");
		QCoreApplication::exit(EFAULT);
	}

    grl::SkeletonExtractorConfig skeletonExtractorConfig;
    skeletonExtractorConfig.depthTolerance = 20;

	SAFE_QT_NEW(_extractor, grl::SkeletonExtractor);
	if (!_extractor->init(skeletonExtractorConfig)) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing extractor");
		QCoreApplication::exit(EFAULT);
	}

    grl::TrackerConfig trackerConfig;
    trackerConfig.filterPoints = 5;
    trackerConfig.framesIdleReset = 2;
    trackerConfig.frameSkip = 1;
    trackerConfig.minDistance = 0.01f;
    trackerConfig.trackingLength = 25;

    SAFE_QT_NEW(_leftTracker, grl::GestureTracker);
    _leftTracker->init(trackerConfig);
    _rightTracker->init(trackerConfig);

	SAFE_QT_NEW(_recognizer, grl::GestureRecognizer);
	if (!_recognizer->init(_kinect, _extractor, _leftTracker, _rightTracker, _orbMatcher)) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing recognizer");
		QCoreApplication::exit(EFAULT);
	}

    loadSampleTracks();

    _ui.setupUi(this);

    //SAFE_QT_NEW(_trackSaver, TrackSaver(this, *_kinect, *_discreteTracker));
    SAFE_QT_NEW(_captureTimer, QTimer(this));

    connect(_ui.recordButton, SIGNAL(clicked()), this, SLOT(recordButtonHandler()));
    connect(_ui.saveTrackButton, SIGNAL(clicked()), this, SLOT(saveTrackButtonHandler()));
    connect(_ui.exrButton, SIGNAL(clicked()), this, SLOT(exrButtonHandler()));

    connect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));
	_captureTimer->start(20);
}

MainWindow::~MainWindow()
{
	delete _kinect;
	delete _orbMatcher;
	delete _extractor;
	delete _recognizer;
    delete _trackSaver;
    delete _captureTimer;
    delete _leftRecorder;
    delete _rightRecorder;
}

void MainWindow::recordButtonHandler()
{
    disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));

    if (_leftRecorder != nullptr || _rightRecorder != nullptr) {
        delete _leftRecorder;
        delete _rightRecorder;
    }

    _leftRecorder = new grl::TrackRecorder(0.02f, 1, 5, 25);
    _rightRecorder = new grl::TrackRecorder(0.02f, 1, 5, 25);

    _recordingLeftFinished = false;
    _recordingRightFinished = false;

    _endTime = QTime::currentTime().addSecs(recordDelay);
    _idleDeadline = _endTime.addSecs(idleDelay);
    connect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordTrack()));
}

void MainWindow::saveTrackButtonHandler()
{
    //QString fileName = QFileDialog::getSaveFileName(this, "Track database file selector");
    //_discreteTracker->writeDatabaseToFile(fileName.toStdString());
}

void MainWindow::exrButtonHandler()
{
    std::string fileName = QFileDialog::getOpenFileName(this, "OpenEXR file select").toStdString();

    if (!fileName.empty()) {
        cv::Mat img = cv::imread(fileName, cv::IMREAD_UNCHANGED);
        int type = img.type();
        int channels = img.channels();
        cv::MatIterator_<cv::Vec3f> it, end;
        float r, g, b;
        for (it = img.begin<cv::Vec3f>(), end = img.end<cv::Vec3f>(); it != end; ++it) {
            b = (*it)[0];
            g = (*it)[1];
            r = (*it)[2];
            if (b != 1.0f || g != 1.0f || r != 1.0f)
                continue;
        }
        cv::Mat_<cv::Vec3f> fimg = img;
        b = fimg(80, 80)[0];
        g = fimg(80, 80)[1];
        r = fimg(80, 80)[2];
    }
}

void MainWindow::recordTrack()
{
    /*
    grl::RecognitionStatus status;
    status = _recognizer->update();
    QTime currentTime = QTime::currentTime();
    const grl::Skeleton &skeleton = _recognizer->getSkeleton();

    int width, height;
    _kinect->getSize(width, height);

    cv::Mat infoimage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    QImage qImage;

    bool canRecord = status != grl::NoInput && status != grl::NoSkeleton;

    if (status != grl::NoInput) {
        cv::Mat depthImage = _recognizer->getNormalizedDepth();

        if (status != grl::NoSkeleton) {
            cv::cvtColor(depthImage, depthImage, CV_GRAY2RGB);
            drawSimpleSkeleton(skeleton, depthImage);
            cvMatRGBToQImage(depthImage, qImage);
        } else {
            cvMatGrayToQImage(depthImage, qImage);
        }

        _ui.depthImageBox->setPixmap(QPixmap::fromImage(qImage));
    }

    // Print info on the right side
    static int idleLeft, idleRight;
    if (currentTime <= _endTime) {
        float countdown = currentTime.msecsTo(_endTime)/1000.0f;
        std::stringstream s;
        s << std::fixed << std::setprecision(1) << countdown;
        cv::String number(s.str());
        cv::putText(infoimage, number, cv::Point(width/4, height/2),
                    CV_FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(255, 0, 0), 4);
        if (!canRecord)
            cv::putText(infoimage, cv::String("Ustaw sie"), cv::Point(20, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 0, 0), 4);
        else
            cv::putText(infoimage, cv::String("OK!"), cv::Point(width/3, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);
    } else {
        if (canRecord) {
            cv::putText(infoimage, cv::String("Nagrywam"), cv::Point(20, height/2),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);

            // Add left hand if recording
            if (!_recordingLeftFinished) {
                grl::TrackState state = _leftRecorder->addPoint(skeleton.joints[grl::LEFT_HAND].coordWorld);

                if (state == grl::grlRecorderPointBuffered || state == grl::grlRecorderPointSkipped) {
                    ++idleLeft;
                } else if (state == grl::grlRecorderPointAdded) {
                    idleLeft = 0;
                    // Draw the track
                    grl::TrackPoints track;
                    grl::offsetsToPoints(_leftRecorder->getTrack(), track);

                    cv::Mat trackImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
                    drawTrack(track.points, cv::Scalar(0, 255, 0), trackImage);

                    cvMatRGBToQImage(trackImage, qImage);
                    _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(qImage));
                }

                if (idleLeft >= maxIdleFrames) {
                    idleLeft = 0;
                    _recordingLeftFinished = true;
                }
            }

            // Add right hand if recording
            if (!_recordingRightFinished) {
                grl::TrackState state = _rightRecorder->addPoint(skeleton.joints[grl::RIGHT_HAND].coordWorld);

                if (state == grl::grlRecorderPointBuffered || state == grl::grlRecorderPointSkipped) {
                    ++idleRight;
                } else if (state == grl::grlRecorderPointAdded) {
                    idleRight = 0;
                    // Draw the track
                    grl::TrackPoints track;
                    grl::offsetsToPoints(_rightRecorder->getTrack(), track);

                    cv::Mat trackImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
                    drawTrack(track.points, cv::Scalar(0, 255, 0), trackImage);

                    cvMatRGBToQImage(trackImage, qImage);
                    _ui.matchedImageBox->setPixmap(QPixmap::fromImage(qImage));
                }

                if (idleRight >= maxIdleFrames) {
                    idleRight = 0;
                    _recordingRightFinished = true;
                }
            }

            _idleDeadline = QTime::currentTime().addSecs(idleDelay);
        } else {
            // If hand was idle for too long, simply end recording
            if (currentTime >= _idleDeadline) {
                idleLeft = 0;
                idleRight = 0;
                _recordingLeftFinished = true;
                _recordingRightFinished = true;
            } else {
                float countdown = currentTime.msecsTo(_idleDeadline)/1000.0f;
                std::stringstream s;
                s << std::fixed << std::setprecision(1) << countdown;
                cv::String number(s.str());
                cv::putText(infoimage, number, cv::Point(width/4, height/2),
                            CV_FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(255, 0, 0), 4);
                cv::putText(infoimage, cv::String("Brak szkieletu"), cv::Point(20, height/2 + 100),
                            CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 0, 0), 4);
            }
        }
    }

    // Debug logs
    std::string debugText;
    debugText += std::string("Left points: ") + std::to_string(_leftRecorder->getLength()) + '\n';
    debugText += std::string("Left idle: ") + std::to_string(idleLeft) + '\n';
    debugText += std::string("Right points: ") + std::to_string(_rightRecorder->getLength()) + '\n';
    debugText += std::string("Right idle: ") + std::to_string(idleRight) + '\n';

    _ui.debugLog->clear();
    _ui.debugLog->setText(QString::fromStdString(debugText));

    cvMatRGBToQImage(infoimage, qImage);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(qImage));
    if (_recordingLeftFinished && _recordingRightFinished) {
        _trackSaver->setTracks(&_leftRecorder->getTrack(), &_rightRecorder->getTrack());
        _trackSaver->exec();

        disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordTrack()));
        connect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));
    }

    */
}

void MainWindow::updateVideoContext()
{
    /*
	grl::RecognitionStatus status;

	status = _recognizer->update();
	if (status == grl::NoInput)
		return;

	QImage image;
	cv::Mat depthImage = _recognizer->getNormalizedDepth();
	cvMatGrayToQImage(depthImage, image);
	_ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

	if (status == grl::NoSkeleton)
		return;

	const grl::Skeleton &skeleton = _recognizer->getSkeleton();

	cv::cvtColor(depthImage, depthImage, CV_GRAY2RGB);

    drawSimpleSkeleton(skeleton, depthImage);

	std::string debugText;
	debugText += std::string("skeleton_lean: ") + std::to_string(skeleton.lean) + '\n';

    // Analyze track
    const grl::OnlineGestureDescriptor &rightOnline = _discreteTracker->getOnlineDescriptorRight();
    const grl::OnlineGestureDescriptor &leftOnline = _discreteTracker->getOnlineDescriptorLeft();

    debugText += std::string("Right-on: ") + static_cast<std::string>(rightOnline.position) + '\n';
    debugText += std::string("Left-on: ") + static_cast<std::string>(leftOnline.position) + '\n';

#if 0
    debugText += std::string("Left-joint: ") + static_cast<std::string>(skeleton.joints[grl::LEFT_HAND].coordWorld) + '\n';
    debugText += std::string("Right-joint: ") + static_cast<std::string>(skeleton.joints[grl::RIGHT_HAND].coordWorld) + '\n';
#endif
    grl::TrackPoints leftTrack;
    grl::TrackPoints rightTrack;
    _discreteTracker->getRawTrackLeft(leftTrack);
    _discreteTracker->getRawTrackRight(rightTrack);

    // Draw track for left hand
    drawTrack(leftTrack.points, cv::Scalar(255, 0, 0), depthImage);

    // Draw track for right hand
    drawTrack(rightTrack.points, cv::Scalar(0, 255, 0), depthImage);

    cvMatRGBToQImage(depthImage, image);
    _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));

    uint8_t recognizedTracks = _discreteTracker->getRecognizedTracks();
    if (recognizedTracks & grl::grlTrackHandLeft)
        _leftTrackDescriptor = _discreteTracker->getOfflineDescriptorLeft();
    if (recognizedTracks & grl::grlTrackHandRight)
        _rightTrackDescriptor = _discreteTracker->getOfflineDescriptorRight();

    if (_leftTrackDescriptor.matchedTrack != nullptr) {
        debugText += std::string("Left-track: ") + *_leftTrackDescriptor.gestureName + '\n';
        debugText += std::string("Error: ") + std::to_string(_leftTrackDescriptor.difference) + '\n';
    }
    if (_rightTrackDescriptor.matchedTrack != nullptr) {
        debugText += std::string("Right-track: ") + *_rightTrackDescriptor.gestureName + '\n';
        debugText += std::string("Error: ") + std::to_string(_rightTrackDescriptor.difference) + '\n';
    }

	if (status == grl::NoHands) {
		_ui.debugLog->clear();
		_ui.debugLog->setText(QString::fromStdString(debugText));
		return;
	}

	cv::Mat hands;
	_recognizer->getHandsImage(hands);
	cvMatGrayToQImage(hands, image);
	_ui.handImageBox->setPixmap(QPixmap::fromImage(image));

    const grl::DepthObject &leftHand = _recognizer->getLeftHand();
    const grl::DepthObject &rightHand = _recognizer->getRightHand();

    debugText += "width: " + std::to_string(rightHand.getBoundingBox().width) + '\n';
    debugText += "max: " + std::to_string(rightHand.getMaxDepthValue()) + '\n';
    debugText += "min: " + std::to_string(rightHand.getMinDepthValue()) + '\n';

	_ui.debugLog->clear();
	_ui.debugLog->setText(QString::fromStdString(debugText));
    */
}


void MainWindow::drawSimpleSkeleton(const grl::Skeleton &skeleton, cv::Mat &image)
{
    // Left hand
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_HAND].coordDepthImage.x, skeleton.joints[grl::LEFT_HAND].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_WRIST].coordDepthImage.x, skeleton.joints[grl::LEFT_WRIST].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_WRIST].coordDepthImage.x, skeleton.joints[grl::LEFT_WRIST].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.x, skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.x, skeleton.joints[grl::LEFT_ELBOW].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::LEFT_SHOULDER].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    // Right hand
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_HAND].coordDepthImage.x, skeleton.joints[grl::RIGHT_HAND].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.x, skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.x, skeleton.joints[grl::RIGHT_WRIST].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.x, skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.x, skeleton.joints[grl::RIGHT_ELBOW].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    cv::line(image,
             cv::Point(skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.x, skeleton.joints[grl::RIGHT_SHOULDER].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));
    // Spine
    cv::line(image,
             cv::Point(skeleton.joints[grl::SPINE_BASE].coordDepthImage.x, skeleton.joints[grl::SPINE_BASE].coordDepthImage.y),
             cv::Point(skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.x, skeleton.joints[grl::SPINE_SHOULDERS].coordDepthImage.y),
             cv::Scalar(0, 0, 255));

    for (int i = 0; i < grl::JointType::COUNT; ++i) {
        cv::Scalar color = skeleton.joints[i].tracked ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 0, 0);
        cv::circle(image,
                   cv::Point(skeleton.joints[i].coordDepthImage.x, skeleton.joints[i].coordDepthImage.y),
                   5, color);
    }
}

void
MainWindow::drawTrack(const std::vector<grl::Vec3f> &track, cv::Scalar color, cv::Mat &image)
{
    if (track.size() < 2)
        return;

    std::vector<grl::Vec2f> track2D;
    _kinect->worldToImage(track, track2D);
    for (auto it = track2D.cbegin() + 1, itBefore = track2D.cbegin(); it != track2D.cend(); itBefore = it++)
            cv::line(image, cv::Point2f(itBefore->x, itBefore->y), cv::Point2f(it->x, it->y), color, 2);
}

void
MainWindow::loadSampleTracks()
{
    /*
    for (int i = 0; i < 2; ++i) {
        auto &sampleNames = i == 0 ? grlSampleRightTracksNames : grlSampleLeftTracksNames;
        auto &samplePointers = i == 0 ? grlSampleRightTracksPointers : grlSampleLeftTracksPointers;
        grl::TrackedHand trackedHand = i == 0 ? grl::grlTrackHandRight : grl::grlTrackHandLeft;

        auto itNames = sampleNames.cbegin();
        auto itPointers = samplePointers.cbegin();
        for (; itPointers != samplePointers.cend(); ++itNames, ++itPointers) {
            auto itSubTrack = (**itPointers).cbegin();
            for (; itSubTrack != (**itPointers).cend(); ++itSubTrack) {
                grl::ProcessedOfflineGestureDescriptor desc = {
                    &(*itNames),
                    std::make_unique<grl::TrackPoints>(),
                    trackedHand
                };
                desc.track->points = *itSubTrack;

                _discreteTracker->addTrack(desc);
            }
        }
    }
    */
}
