#include "GestureSaver.h"
#include "QTUtils.h"

#include <QMessageBox>

#include <grl/utils/ImageToolkit.h>
#include <grl/track/TrackTools.h>

#include <filesystem>

GestureSaver::GestureSaver(QWidget *parent)
    : QDialog(parent)
{
    _ui.setupUi(this);

    connect(_ui.okButton, SIGNAL(released()), this, SLOT(saveGestureHandler()));
    connect(_ui.cancelButton, SIGNAL(released()), this, SLOT(reject()));
}

#define UI_PREVIEW(ui, n) ((ui).handPreview##n)

int GestureSaver::exec()
{
    const std::vector<QLabel *> labels = {
        UI_PREVIEW(_ui, 1),
        UI_PREVIEW(_ui, 2),
        UI_PREVIEW(_ui, 3),
        UI_PREVIEW(_ui, 4),
        UI_PREVIEW(_ui, 5),
        UI_PREVIEW(_ui, 6),
        UI_PREVIEW(_ui, 7),
        UI_PREVIEW(_ui, 8),
        UI_PREVIEW(_ui, 9),
    };
    QImage image;
    _ui.retranslateUi(this);

    int i = 0;
    auto itl = labels.cbegin();
    for (auto it = _handClasses->cbegin(); it != _handClasses->cend(); ++it, ++itl) {
        cvMatRGBToQImage(*it, image);
        (*itl)->setPixmap(QPixmap::fromImage(image));
    }

    return QDialog::exec();
}

#define UI_CHECK(ui, n) ((ui).checkHand##n)

void
GestureSaver::saveGestureHandler()
{
    const std::vector<QCheckBox *> boxes = {
        UI_CHECK(_ui, 1),
        UI_CHECK(_ui, 2),
        UI_CHECK(_ui, 3),
        UI_CHECK(_ui, 4),
        UI_CHECK(_ui, 5),
        UI_CHECK(_ui, 6),
        UI_CHECK(_ui, 7),
        UI_CHECK(_ui, 8),
        UI_CHECK(_ui, 9),
    };

    std::string baseName = _ui.inGestureName->toPlainText().toStdString();
    if (baseName.empty()) {
        QMessageBox::warning(this, "Save gesture error", "The name of the gesture cannot be empty", QMessageBox::StandardButton::Ok);
        return;
    }

    // Create the directory if needed
    std::experimental::filesystem::path dir("gestures");
    if (!(std::experimental::filesystem::exists(dir))) {
        if (!std::experimental::filesystem::create_directory(dir)) {
            QMessageBox::critical(this, "Directory error", "Could not create directory for saving gestures. Please create it by hand ('gestures' folder where the .exe resides)", QMessageBox::StandardButton::Ok);
            return;
        }
    }

    auto pose = _poseCount.insert(std::pair<std::string, size_t>(baseName, 0));
    auto hand = _hands->cbegin();
    auto check = boxes.cbegin();
    auto img = _handClasses->cbegin();
    for (; hand != _hands->cend(); ++hand, ++check, ++img) {
        if (!(*check)->isChecked())
            continue;

        // Get the index
        size_t id = ++pose.first->second;
        // Save the image to file
        std::ostringstream finalName;
        finalName << "gestures/" << baseName << id;
        cv::Mat handImage;
        cvtColor(*img, handImage, cv::COLOR_RGB2BGR);
        cv::imwrite(cv::String(finalName.str() + ".png"), handImage);

        // Save gesture to file
        if (!grl::saveHandSkeletonToFile(*hand, finalName.str() + ".dat")) {
            QMessageBox::critical(this, "Save error", "Failed to save the gesture to file", QMessageBox::StandardButton::Ok);
            --pose.first->second;
            return;
        }
    }

    accept();
}
