#include "TrackSaver.h"
#include "QTUtils.h"

#include <QMessageBox>

TrackSaver::TrackSaver(QWidget *parent,
                       const grl::DepthCamera &camera,
                       grl::GestureTracker &tracker,
                       const grl::TrackOffsets *leftTrack,
                       const grl::TrackOffsets *rightTrack)
    : QDialog(parent)
    , _leftTrack(leftTrack)
    , _rightTrack(rightTrack)
    , _camera(camera)
    , _tracker(tracker)
{
    _ui.setupUi(this);

    connect(_ui.okButton, SIGNAL(released()), this, SLOT(saveTrackHandler()));
    connect(_ui.cancelButton, SIGNAL(released()), this, SLOT(reject()));
}

int
TrackSaver::exec()
{
    _ui.retranslateUi(this);

    /* TODO - convert the code for the new API
    _tracker.processTrack(*_leftTrack, _leftTrackProcessed);
    _tracker.processTrack(*_rightTrack, _rightTrackProcessed);

    QImage qImage;
    int width, height;
    _camera.getSize(width, height);
    cv::Mat leftTrackImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat rightTrackImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

    grl::TrackPoints translatedTrackLeft = _leftTrackProcessed + _leftTrack->startingPoint;
    grl::TrackPoints translatedTrackRight = _rightTrackProcessed + _rightTrack->startingPoint;

    std::vector<grl::Vec2f> track2D;
    // Left track
    _camera.worldToImage(translatedTrackLeft.points, track2D);
    if (track2D.size() > 1)
        for (auto it = track2D.cbegin() + 1, itBefore = track2D.cbegin(); it != track2D.cend(); itBefore = it++)
            cv::line(leftTrackImage, cv::Point2f(itBefore->x, itBefore->y), cv::Point2f(it->x, it->y), cv::Scalar(0, 255, 0), 2);
    for (auto it = track2D.cbegin(); it != track2D.cend(); it++)
        cv::circle(leftTrackImage, cv::Point(it->x, it->y), 5, cv::Scalar(0, 0, 255), 2);

    // Right track
    _camera.worldToImage(translatedTrackRight.points, track2D);
    if (track2D.size() > 1)
        for (auto it = track2D.cbegin() + 1, itBefore = track2D.cbegin(); it != track2D.cend(); itBefore = it++)
            cv::line(rightTrackImage, cv::Point2f(itBefore->x, itBefore->y), cv::Point2f(it->x, it->y), cv::Scalar(0, 255, 0), 2);
    for (auto it = track2D.cbegin(); it != track2D.cend(); it++)
        cv::circle(rightTrackImage, cv::Point(it->x, it->y), 5, cv::Scalar(0, 0, 255), 2);

    // Display images
    cvMatRGBToQImage(leftTrackImage, qImage);
    _ui.leftTrackPreview->setPixmap(QPixmap::fromImage(qImage));

    cvMatRGBToQImage(rightTrackImage, qImage);
    _ui.rightTrackPreview->setPixmap(QPixmap::fromImage(qImage));

    // Fill labels
    // Left
    _ui.labLeftBegin->setText(QString::fromStdString(translatedTrackLeft.points.front()));
    _ui.labLeftEnd->setText(QString::fromStdString(translatedTrackLeft.points.back()));
    _ui.labLeftLen->setText(QString::fromStdString(std::to_string(_leftTrack->getLength())));
    _ui.labLeftPoints->setText(QString::fromStdString(std::to_string(_leftTrackProcessed.points.size())));

    // Right
    _ui.labRightBegin->setText(QString::fromStdString(translatedTrackRight.points.front()));
    _ui.labRightEnd->setText(QString::fromStdString(translatedTrackRight.points.back()));
    _ui.labRightLen->setText(QString::fromStdString(std::to_string(_rightTrack->getLength())));
    _ui.labRightPoints->setText(QString::fromStdString(std::to_string(_rightTrackProcessed.points.size())));
    */

    return QDialog::exec();
}

void
TrackSaver::saveTrackHandler()
{
    /*
    std::string trackName = _ui.inTrackName->toPlainText().toStdString();
    if (trackName.empty()) {
        QMessageBox::warning(this, "Save track error", "The name of the track cannot be empty", QMessageBox::StandardButton::Ok);
        return;
    }

    QString message;
    if (_ui.checkSaveLeft->isChecked()) {
        grl::NewOfflineGestureDescriptor descriptor = {&trackName, _leftTrack, grl::grlTrackHandLeft};
        if (_tracker.addTrack(descriptor))
            message += "Left track appended to already existing track\n";
        else
            message += "New track added for left hand\n";
    }

    if (_ui.checkSaveRight->isChecked()) {
        grl::NewOfflineGestureDescriptor descriptor = {&trackName, _rightTrack, grl::grlTrackHandRight};
        if (_tracker.addTrack(descriptor))
            message += "Right track appended to already existing track\n";
        else
            message += "New track added for right hand\n";
    }

    if (message.isEmpty())
        message = "Nothing happened";

    QMessageBox::information(this, "Save track", message, QMessageBox::StandardButton::Ok);

    accept();
    */
}
