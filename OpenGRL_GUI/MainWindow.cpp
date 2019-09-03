#include "MainWindow.h"

#include <grl/rdf/RandomDecisionForest.h>

#include <grl/utils/ImageToolkit.h>
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

    grl::SkeletonExtractorConfig skeletonExtractorConfig;
    skeletonExtractorConfig.depthTolerance = 20;

	SAFE_QT_NEW(_extractor, grl::SkeletonExtractor);
	if (!_extractor->init(skeletonExtractorConfig)) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing extractor");
		QCoreApplication::exit(EFAULT);
	}

    grl::TrackerConfig trackerConfig;
    trackerConfig.filterPoints = 3;
    trackerConfig.framesIdleReset = 10;
    trackerConfig.frameSkip = 2;
    trackerConfig.minDistance = 0.03f;
    trackerConfig.trackingLength = 50;

    SAFE_QT_NEW(_rightTracker, grl::GestureTracker);
    _rightTracker->init(trackerConfig);

    SAFE_QT_NEW(_trackKNN, grl::DiscretizedTrackClassification<14>);
    _trackKNN->init(5);

    SAFE_QT_NEW(_rdfExtractor, grl::RDFHandSkeletonExtractor);
    if (!_rdfExtractor->init("../resources/rdf.txt", _kinect)) {
        std::cout << "Invalid forest" << std::endl;
        QCoreApplication::exit(EFAULT);
    }

    SAFE_QT_NEW(_gestureKNN, grl::BoneOrientationClassificator);
    _gestureKNN->init(3, 37);

    SAFE_QT_NEW(_recognizer, grl::GestureRecognizer);

	if (!_recognizer->init(_kinect, _extractor, _rightTracker, _trackKNN, _rdfExtractor, _gestureKNN)) {
		QMessageBox::critical(this, "Fatal error", "Error when intitializing recognizer");
		QCoreApplication::exit(EFAULT);
	}

    loadSampleTracks();

    _ui.setupUi(this);

    SAFE_QT_NEW(_trackSaver, TrackSaver(this, *_kinect));
    SAFE_QT_NEW(_gestureSaver, GestureSaver(this));
    SAFE_QT_NEW(_captureTimer, QTimer(this));

    connect(_ui.recordButton, SIGNAL(clicked()), this, SLOT(recordButtonHandler()));
    connect(_ui.saveGestureButton, SIGNAL(clicked()), this, SLOT(recordGestureButtonHandler()));
    connect(_ui.saveProfileButton, SIGNAL(clicked()), this, SLOT(saveProfileButtonHandler()));
    connect(_ui.certainty, &QSlider::valueChanged, 
            this, &MainWindow::updateSliderHandler);
    updateSliderHandler(_ui.certainty->value());

    connect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));
	_captureTimer->start(20);
}

MainWindow::~MainWindow()
{
	delete _kinect;
	delete _extractor;
	delete _recognizer;
    delete _trackSaver;
    delete _gestureSaver;
    delete _captureTimer;
    delete _rightTracker;
    delete _trackKNN;
    delete _rdfExtractor;
}

void MainWindow::saveProfileButtonHandler()
{
    std::ofstream file("profile.log", std::ofstream::trunc | std::ofstream::out);
    grl::profiler.reportShort(file);
}

void MainWindow::updateSliderHandler(int value)
{
    _certaintyValue = value / 5.0f; // / (5.0f*_ui.certainty->maximum());
    _ui.certaintyValue->setText(QString::fromStdString(std::to_string(_certaintyValue)));
}

void MainWindow::recordButtonHandler()
{
    disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));

    _rightTracker->clear();

    _recordingFinished = false;

    _endTime = QTime::currentTime().addSecs(recordDelay);
    connect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordTrack()));
}

void MainWindow::recordGestureButtonHandler()
{
    disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));

    _recordingFinished = false;
    _handImages.clear();
    _handSkeletons.clear();

    _endTime = QTime::currentTime().addSecs(recordDelay);
    connect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordGesture()));
}

void MainWindow::drawDepthWithSkeleton()
{
    QImage image;
    if (_lastStatus & grl::GotDepth) {
        cv::Mat depthImage = _recognizer->getNormalizedDepth();
        const grl::Skeleton &skeleton = _recognizer->getSkeleton();

        if (_lastStatus & grl::GotSkeleton) {
            cv::cvtColor(depthImage, depthImage, CV_GRAY2RGB);
            drawSimpleSkeleton(skeleton, depthImage);
            cvMatRGBToQImage(depthImage, image);
        } else {
            cvMatGrayToQImage(depthImage, image);
        }

        _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::recordGesture()
{
    QImage image;
    static bool recordingStarted = false;

    _lastStatus = _recognizer->update(recordingStarted ?
                                      grl::RecognitionMode::CaptureGesture :
                                      grl::RecognitionMode::GetSkeleton
    );
    if (_lastStatus == grl::GotNothing)
        return;

    QTime currentTime = QTime::currentTime();
    const grl::Skeleton &skeleton = _recognizer->getSkeleton();

    int width, height;
    _kinect->getSize(width, height);

    cv::Mat infoImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

    bool canRecord = (_lastStatus & grl::GotGesture) != 0;

    drawDepthWithSkeleton();

    // Print info on the right side
    if (currentTime <= _endTime) {
        drawCountdown(currentTime, _endTime, infoImage);
        if (!canRecord)
            cv::putText(infoImage, cv::String("Ustaw sie"), cv::Point(20, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 0, 0), 4);
        else
            cv::putText(infoImage, cv::String("OK!"), cv::Point(width/3, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);
    } else {
        if (!recordingStarted) {
            recordingStarted = true;
            return;
        }
        if (canRecord) {
            cv::putText(infoImage, cv::String("Nagrywam"), cv::Point(20, height/2),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);

            // Add left hand if recording
            if (!_recordingFinished) {
                static int n;
                grl::HandSkeleton handSkeleton = _recognizer->getHandSkeleton();
                cv::Mat calculatedClassesRGB;
                cv::Mat &assignedClasses = _rdfExtractor->getLastClasses();
                grl::RDFTools::convertHandClassesToRGB(assignedClasses, calculatedClassesRGB);
                grl::drawJointsOnImage(handSkeleton, calculatedClassesRGB, _certaintyValue);
                cvMatRGBToQImage(calculatedClassesRGB, image);
                _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));
                // Save only every 5 frame, to give the user more control
                if (++n == 5) {
                    n = 0;
                    _handImages.push_back(calculatedClassesRGB);
                    _handSkeletons.push_back(handSkeleton);

                    if (_handImages.size() == 9)
                        _recordingFinished = true;
                }
            }
        } else {
            cv::putText(infoImage, cv::String("Brak szkieletu"), cv::Point(20, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 0, 0), 4);
        }
    }

    _ui.debugLog->clear();

    cvMatRGBToQImage(infoImage, image);
    _ui.matchedImageBox->setPixmap(QPixmap::fromImage(image));
    if (_recordingFinished) {
        recordingStarted = false;
        
        _gestureSaver->setGestures(&_handImages, &_handSkeletons);
        _gestureSaver->exec();

        disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordGesture()));
        connect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));
    }
}

void MainWindow::recordTrack()
{
    QImage image;
    static bool recordingStarted = false;

    _lastStatus = _recognizer->update(recordingStarted ? 
                                      grl::RecognitionMode::CaptureTrack : 
                                      grl::RecognitionMode::GetSkeleton
    );
    if (_lastStatus == grl::GotNothing)
        return;

    QTime currentTime = QTime::currentTime();
    const grl::Skeleton &skeleton = _recognizer->getSkeleton();

    int width, height;
    _kinect->getSize(width, height);

    cv::Mat infoImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

    bool canRecord = (_lastStatus & grl::GotSkeleton) != 0;

    drawDepthWithSkeleton();

    // Print info on the right side
    if (currentTime <= _endTime) {
        drawCountdown(currentTime, _endTime, infoImage);
        if (!canRecord)
            cv::putText(infoImage, cv::String("Ustaw sie"), cv::Point(20, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 0, 0), 4);
        else
            cv::putText(infoImage, cv::String("OK!"), cv::Point(width/3, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);
    } else {
        if (!recordingStarted) {
            _rightTracker->clear();
            recordingStarted = true;
            return;
        }
        if (canRecord) {
            cv::putText(infoImage, cv::String("Nagrywam"), cv::Point(20, height/2),
                        CV_FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 0), 4);

            // Add left hand if recording
            if (!_recordingFinished) {
                grl::TrackPoints track;
                cv::Mat trackImage;
                switch (_recognizer->getLastTrackerState()) {
                case(grl::GestureTracker::grlTrackerAdded):
                    _rightTracker->getCurrentTrack(track);
                    trackImage = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
                    drawTrack(track, trackImage, *_kinect);
                    cvMatRGBToQImage(trackImage, image);
                    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
                    break;
                case(grl::GestureTracker::grlTrackerReset):
                    // Stop recording if have at least 2 points
                    if (_rightTracker->getLastTrack().getPointsCount() > 1)
                        _recordingFinished = true;
                    break;
                }
            }
        } else {
            cv::putText(infoImage, cv::String("Brak szkieletu"), cv::Point(20, height/2 + 100),
                        CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 0, 0), 4);
        }
    }

    _ui.debugLog->clear();

    cvMatRGBToQImage(infoImage, image);
    _ui.matchedImageBox->setPixmap(QPixmap::fromImage(image));
    if (_recordingFinished) {
        recordingStarted = false;
        _trackSaver->setTrack(&_rightTracker->getLastTrack());
        _trackSaver->exec();

        disconnect(_captureTimer, SIGNAL(timeout()), this, SLOT(recordTrack()));
        connect(_captureTimer, SIGNAL(timeout()), this, SLOT(updateVideoContext()));
    }
}

void MainWindow::all()
{
    std::ostringstream debugText;
    QImage image;


	_lastStatus = _recognizer->update(grl::All);
    if (_lastStatus == grl::GotNothing)
        return;

    drawDepthWithSkeleton();

    grl::TrackPoints rightTrack;
    _rightTracker->getCurrentTrack(rightTrack);

    int width, height;
    _kinect->getSize(width, height);
    // Draw track for right hand
    cv::Mat mat = cv::Mat::zeros(height, width, CV_8UC3);
    grl::drawTrack(rightTrack, mat, *_kinect);
    cvMatRGBToQImage(mat, image);
    _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));

    if (_lastStatus & grl::GotGesture) {
        cv::Mat hands = _rdfExtractor->getLastClasses();
        cv::Mat calculatedClassesRGB;
        grl::RDFTools::convertHandClassesToRGB(hands, calculatedClassesRGB);
        grl::drawJointsOnImage(_recognizer->getHandSkeleton(), calculatedClassesRGB, _certaintyValue);
        cvMatRGBToQImage(calculatedClassesRGB, image);
        _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
    }
    auto tmatch = _recognizer->getTrackMatch();
    debugText << "Track: " << *tmatch.trackCategory << " " << tmatch.score1 << std::endl;

    auto gmatch = _recognizer->getGestureMatch();
    debugText << "Gesture: " << *gmatch.gestureCategory << " " << gmatch.score1 << std::endl;
    
    _ui.debugLog->clear();
	_ui.debugLog->setText(QString::fromStdString(debugText.str()));
}

void MainWindow::updateVideoContext()
{
    //testDepthAndColorize();
    //testDepthRanges();
    //testDepthMedian();
    //testTrackCapturing();
    //testHandExtract();
    //testHandClassify();

    // Perform all steps - extraction and classification of all gestures.
    all();
}


template<size_t s>
bool learnTrackClassificator(grl::DiscretizedTrackClassification<s> &classificator)
{
    static const std::string baseFolderLearn("tracks-learn");
    static constexpr std::array<const char *, 7> baseNames = {
        "a",
        "c",
        "m",
        "machanie",
        "zamach",
        "n",
        "notu",
    };
    static constexpr size_t learnSamples = 6;
    for (auto it = baseNames.cbegin(); it != baseNames.cend(); ++it) {
        for (size_t i = 1; i <= learnSamples; ++i) {
            std::ostringstream file;
            file << baseFolderLearn << "/" << *it << i << ".dat";

            grl::TrackPoints track;
            if (!grl::TrackTools::loadTrackFromFile(track, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            classificator.updateDatabase(*it, track);
        }
    }

    return true;
}

bool learnGestureClassificator(grl::BoneOrientationClassificator &classificator)
{
    static const std::string basegFolderLearn("gestures-learn");
    static constexpr std::array<const char *, 8> basegNames = {
        "nie",
        "ok",
        "tak",
        "piesc",
        "pokoj",
        "relaks",
        "tyl",
        "stop",
    };
    static constexpr size_t learngSamples = 6;

    for (auto it = basegNames.cbegin(); it != basegNames.cend(); ++it) {
        for (size_t i = 1; i <= learngSamples; ++i) {
            std::ostringstream file;
            file << basegFolderLearn << "/" << *it << i << ".dat";

            grl::HandSkeleton skeleton;
            if (!grl::loadHandSkeletonFromFile(skeleton, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            classificator.updateDatabase(*it, skeleton);
        }
    }

    return true;
}

void
MainWindow::loadSampleTracks()
{
    learnTrackClassificator(*_trackKNN);
    learnGestureClassificator(*_gestureKNN);
}

// Below we can find the testing functions that were using for acquring sample images etc.
void MainWindow::testDepthAndColorize()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    depthFrame = grl::DepthToColor(depthFrame, 500, 3000);
    cvMatGrayToQImage(depthFrame, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    cv::Mat colorized;
    _kinect->colorizeDepth(colorized);
    cvMatRGBaToQImage(colorized, image);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::testDepthRanges()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    cv::Mat depthHigh = grl::DepthToColor(depthFrame, 500, 4500);
    cvMatGrayToQImage(depthHigh, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    cv::Mat depthLow = grl::DepthToColor(depthFrame, 800, 1100);
    cvMatGrayToQImage(depthLow, image);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::testDepthMedian()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    cv::Mat depth = grl::DepthToColor(depthFrame, 800, 1100);
    cvMatGrayToQImage(depth, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    grl::EnhanceDepthFrame(depthFrame);
    cv::Mat depthMedian = grl::DepthToColor(depthFrame, 800, 1100);
    cvMatGrayToQImage(depthMedian, image);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::testTrackCapturing()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    grl::EnhanceDepthFrame(depthFrame);

    cv::Mat depth = grl::DepthToColor(depthFrame, 500, 4500);
    cvMatGrayToQImage(depth, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    grl::Skeletons skeletons;
    if (!_kinect->getSkeletons(&skeletons))
        return;

    if (skeletons.empty())
        return;

    std::vector<grl::Skeleton *> validSkeletons;
    for (auto it = skeletons.begin(); it != skeletons.end(); ++it)
        if (it->lean <= 50.0f) validSkeletons.push_back(&(*it));

    if (validSkeletons.empty())
        return;

    grl::Skeleton *closestSkeleton = *validSkeletons.begin();
    for (auto it = validSkeletons.begin() + 1; it != validSkeletons.end(); ++it)
        if (closestSkeleton->distance > (*it)->distance) closestSkeleton = *it;

    if (_rightTracker->update(closestSkeleton->joints[grl::RIGHT_HAND]) == grl::GestureTracker::grlTrackerAdded) {
        grl::TrackPoints track;
        _rightTracker->getCurrentTrack(track);

        cv::Mat depthTrack;
        cv::cvtColor(depth, depthTrack, CV_GRAY2RGB);
        drawTrack(track, depthTrack, *_kinect);
        cvMatRGBToQImage(depthTrack, image);
        _ui.handImageBox->setPixmap(QPixmap::fromImage(image));

        cv::Mat onlyTrack = cv::Mat::zeros(depth.rows, depth.cols, CV_8UC3);
        drawTrack(track, onlyTrack, *_kinect);
        cvMatRGBToQImage(onlyTrack, image);
        _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::testHandExtract()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    grl::EnhanceDepthFrame(depthFrame);

    cv::Mat depth = grl::DepthToColor(depthFrame, 500, 4500);
    cvMatGrayToQImage(depth, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    grl::Skeletons skeletons;
    if (!_kinect->getSkeletons(&skeletons))
        return;

    if (skeletons.empty())
        return;

    std::vector<grl::Skeleton *> validSkeletons;
    for (auto it = skeletons.begin(); it != skeletons.end(); ++it)
        if (it->lean <= 50.0f) validSkeletons.push_back(&(*it));

    if (validSkeletons.empty())
        return;

    grl::Skeleton *closestSkeleton = *validSkeletons.begin();
    for (auto it = validSkeletons.begin() + 1; it != validSkeletons.end(); ++it)
        if (closestSkeleton->distance > (*it)->distance) closestSkeleton = *it;

    cv::cvtColor(depth, depth, CV_GRAY2RGB);
    drawSimpleSkeleton(*closestSkeleton, depth);
    cvMatRGBToQImage(depth, image);
    _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));

    grl::DepthObject leftHand, rightHand;
    _extractor->extractHands(depthFrame, *closestSkeleton, leftHand, rightHand);

    cv::Mat destination;
    int rows, cols;
    _kinect->getSize(cols, rows);
    cv::Mat tmp = cv::Mat::zeros(rows, cols, CV_16UC1);
    destination = cv::Mat::zeros(rows, cols, CV_16UC1);

    if (leftHand.getAccuracy() > 0)
        leftHand.getDepthImageOfObject().copyTo(tmp(leftHand.getBoundingBox()));
    if (rightHand.getAccuracy() > 0)
        rightHand.getDepthImageOfObject().copyTo(destination(rightHand.getBoundingBox()));

    cv::Mat result = tmp + destination;

    cv::normalize(result, result, 0, UINT8_MAX, cv::NORM_MINMAX, CV_8UC1);

    result = UINT8_MAX - result;

    cvMatGrayToQImage(result, image);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::testHandClassify()
{
    cv::Mat depthFrame;
    QImage image;

    if (!_kinect->getFrame(&depthFrame))
        return;

    grl::EnhanceDepthFrame(depthFrame);

    cv::Mat depth = grl::DepthToColor(depthFrame, 500, 4500);
    cvMatGrayToQImage(depth, image);
    _ui.depthImageBox->setPixmap(QPixmap::fromImage(image));

    grl::Skeletons skeletons;
    if (!_kinect->getSkeletons(&skeletons))
        return;

    if (skeletons.empty())
        return;

    std::vector<grl::Skeleton *> validSkeletons;
    for (auto it = skeletons.begin(); it != skeletons.end(); ++it)
        if (it->lean <= 50.0f) validSkeletons.push_back(&(*it));

    if (validSkeletons.empty())
        return;

    grl::Skeleton *closestSkeleton = *validSkeletons.begin();
    for (auto it = validSkeletons.begin() + 1; it != validSkeletons.end(); ++it)
        if (closestSkeleton->distance > (*it)->distance) closestSkeleton = *it;


    cv::cvtColor(depth, depth, CV_GRAY2RGB);
    drawSimpleSkeleton(*closestSkeleton, depth);
    cvMatRGBToQImage(depth, image);
    _ui.skeletonImageBox->setPixmap(QPixmap::fromImage(image));

    grl::DepthObject leftHand, rightHand;
    _extractor->extractHands(depthFrame, *closestSkeleton, leftHand, rightHand);

    cv::Mat destination;
    int rows, cols;
    _kinect->getSize(cols, rows);
    cv::Mat tmp = cv::Mat::zeros(rows, cols, CV_16UC1);
    destination = cv::Mat::zeros(rows, cols, CV_16UC1);

    if (rightHand.getAccuracy() == 0)
        return;

    rightHand.getDepthImageOfObject().copyTo(destination(rightHand.getBoundingBox()));

    cv::Mat result = tmp + destination;

    cv::normalize(result, result, 0, UINT8_MAX, cv::NORM_MINMAX, CV_8UC1);

    result = UINT8_MAX - result;

    cvMatGrayToQImage(result, image);
    _ui.handImageBox->setPixmap(QPixmap::fromImage(image));

    grl::HandSkeleton handSkeleton;
    _rdfExtractor->extractSkeleton(rightHand, handSkeleton);

    cv::Mat calculatedClassesRGB;
    cv::Mat assignedClasses = _rdfExtractor->getLastClasses();
    grl::RDFTools::convertHandClassesToRGB(assignedClasses, calculatedClassesRGB);
    grl::drawJointsOnImage(handSkeleton, calculatedClassesRGB, _certaintyValue);
    cvMatRGBToQImage(calculatedClassesRGB, image);
    _ui.matchedImageBox->setPixmap(QPixmap::fromImage(image));
}
