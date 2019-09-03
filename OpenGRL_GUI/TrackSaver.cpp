#include "TrackSaver.h"
#include "QTUtils.h"

#include <QMessageBox>

#include <grl/utils/ImageToolkit.h>
#include <grl/track/TrackTools.h>

#include <filesystem>

TrackSaver::TrackSaver(QWidget *parent, const grl::DepthCamera &camera)
    : QDialog(parent)
    , _camera(camera)
{
    _ui.setupUi(this);

    connect(_ui.okButton, SIGNAL(released()), this, SLOT(saveTrackHandler()));
    connect(_ui.cancelButton, SIGNAL(released()), this, SLOT(reject()));
}

int
TrackSaver::exec()
{
    QImage image;
    _ui.retranslateUi(this);

    int width, height;
    _camera.getSize(width, height);

    _trackImage = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    grl::drawTrack(*_track, _trackImage, _camera);

    // Display images
    cvMatRGBToQImage(_trackImage, image);
    _ui.trackPreview->setPixmap(QPixmap::fromImage(image));

    // Fill labels
    _ui.labBegin->setText(
        QString::fromStdString(std::string(_track->getElement(0)))
    );
    _ui.labEnd->setText(
        QString::fromStdString(std::string(_track->getLastPoint()))
    );
    _ui.labLen->setText(
        QString::fromStdString(std::to_string(_track->getTrackLength()))
    );
    _ui.labPoints->setText(
        QString::fromStdString(std::to_string(_track->getPointsCount()))
    );

    return QDialog::exec();
}

void
TrackSaver::saveTrackHandler()
{
    std::string baseName = _ui.inTrackName->toPlainText().toStdString();
    if (baseName.empty()) {
        QMessageBox::warning(this, "Save track error", "The name of the track cannot be empty", QMessageBox::StandardButton::Ok);
        return;
    }

    // Create the directory if needed
    std::experimental::filesystem::path dir("tracks");
    if (!(std::experimental::filesystem::exists(dir))) {
        if (!std::experimental::filesystem::create_directory(dir)) {
            QMessageBox::critical(this, "Directory error", "Could not create directory for saving tracks. Please create it by hand ('tracks' folder where the .exe resides)", QMessageBox::StandardButton::Ok);
            return;
        }
    }

    auto pose = _poseCount.insert(std::pair<std::string, size_t>(baseName, 0));
    // Get the index
    size_t id = ++pose.first->second;

    // Save the image to file
    std::ostringstream finalName;
    finalName << "tracks/" << baseName << id; 
    cvtColor(_trackImage, _trackImage, cv::COLOR_RGB2BGR);
    cv::imwrite(cv::String(finalName.str() + ".png"), _trackImage);

    // Save the track to file
    if (!grl::TrackTools::saveTrackToFile(*_track, finalName.str() + ".dat")) {
        QMessageBox::critical(this, "Save error", "Failed to save the track to file", QMessageBox::StandardButton::Ok);
        --pose.first->second;
        return;
    }

    accept();
}
