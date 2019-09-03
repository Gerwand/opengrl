#include "ImageWindow.h"
#include <grl/gesture/RDFHandSkeletonExtractor.h>
#include <grl/gesture/GestureClassificator.h>
#include <grl/rdf/RDFUtils.h>
#include <grl/camera/KinectCamera.h>
#include <grl/track/TrackClassificator.h>

namespace grl {

// Better do not use this one if one does not have the images from blender
void pixelClassStats()
{
    grl::RandomDecisionForest forest;
    if (!forest.loadFromFile("../resources/rdf.txt")) {
        std::cout << "Invalid forest" << std::endl;
        exit(1);
    }

    std::map<size_t, std::pair<size_t, size_t> > poseStats;
    size_t npose = 0;
    for (size_t i = 1; i < 24000; i+= 2000, ++npose) {
        std::vector<cv::Mat> classImages;
        std::vector<cv::Mat> depthImages;

        std::cout << "Loading images for pose "
                  << npose
                  << ", and images in range: "
                  << i
                  << ".."
                  << i+200
                  << std::endl;

        RDFTools::loadDepthImagesWithClasses(
            i, i+200, 2, 7,
            "../../generated-train-small/hand_classes_", // png
            "../../generated-train-small/hand_depth_", // png
            classImages, depthImages
        );

        std::cout << "Image loading done, checking score...\n";

        size_t totalPixels = 0;
        size_t matched = 0;
        for (size_t n = 0; n < 100; ++n) {
            cv::Mat classImage = classImages[n];
            cv::Mat depthImage = depthImages[n];

            cv::Mat foundClasses;
            ClassesWeights weights; ClassesPoints points;
            forest.classifyImage(depthImage, foundClasses, weights, points);

            for (auto foundPixel = foundClasses.begin<int8_t>(), referencePixel = classImage.begin<int8_t>();
                 foundPixel != foundClasses.end<int8_t>();
                 ++foundPixel, ++referencePixel)
            {
                if (*foundPixel == grlBackgroundIndex)
                    continue;
                ++totalPixels;
                if (*foundPixel == *referencePixel)
                    ++matched;
            }
        }
        poseStats[npose] = std::make_pair(matched, totalPixels);
    }

    for (auto it = poseStats.begin(); it != poseStats.end(); ++it) {
        std::cout << "Stats for pose " << it->first << ":" << std::endl;
        std::cout << "\tmatched "
                  << it->second.first
                  << "/"
                  << it->second.second
                  << " "
                  << static_cast<double>(it->second.first) / it->second.second * 100.0f
                  << "%"
                  << std::endl;
    }
}

static const std::string baseFolderLearn("tracks-learn");
static const std::string baseFolderTest("tracks-test");
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
static constexpr size_t testSamples = 10;

static std::ofstream fout;

template<size_t s>
bool learnTrackClassificator(DiscretizedTrackClassification<s> &classificator)
{
    for (auto it = baseNames.cbegin(); it != baseNames.cend(); ++it) {
        for (size_t i = 1; i <= learnSamples; ++i) {
            std::ostringstream file;
            file << baseFolderLearn << "/" << *it << i << ".dat";

            TrackPoints track;
            if (!TrackTools::loadTrackFromFile(track, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            classificator.updateDatabase(*it, track);
        }
    }

    return true;
}

using TrackCatalogue = NamesCatalogue<TrackPoints>;

bool getTestSamples(TrackCatalogue &catalogue)
{
    for (auto it = baseNames.cbegin(); it != baseNames.cend(); ++it) {
        for (size_t i = 1; i <= testSamples; ++i) {
            std::ostringstream file;
            file << baseFolderTest << "/" << *it << i << ".dat";

            auto track = std::make_unique<TrackPoints>();
            if (!TrackTools::loadTrackFromFile(*track, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            catalogue.addElement(*it, std::move(track));
        }
    }

    return true;
}

template<size_t s, size_t knn>
void testTracks()
{
    DiscretizedTrackClassification<s> classificator;
    TrackCatalogue test;

    fout << "\n\n############# Testing for knn " << knn << " and segs " << s << std::endl;

    classificator.init(knn);
    if (!learnTrackClassificator(classificator)) {
        std::cerr << "Failed to learn classificator" << std::endl;
        exit(-1);
    }
    if (!getTestSamples(test)) {
            std::cerr << "Failed to get test samples" << std::endl;
            exit(1);
    }

    std::map<std::string, size_t> goodMatches;

    for (auto it = test.cbegin(); it != test.cend(); ++it) {
        const std::string &myClass = it->first;
        goodMatches[myClass] = 0;
        for (auto itt = it->second.cbegin(); itt != it->second.cend(); ++itt) {
            const TrackPoints &track = **itt;
            auto desc = classificator.recognize(track);
            fout
                << "Recognizing " << myClass
                << ": score " << desc.score1
                << ", class " << *desc.trackCategory
                << ", matches " << desc.score2
                << std::endl;
            if (myClass == *desc.trackCategory)
                ++goodMatches[myClass];
        }
    }

    size_t total = 0;
    for (auto it = goodMatches.cbegin(); it != goodMatches.cend(); ++it) {
        total += it->second;
        fout
            << it->first << ": "
            << it->second << "/" << testSamples
            << " " << it->second*100.0f/testSamples << "%"
            << std::endl;
    }

    fout
        << "Total efficiency: "
        << total*100.0f/(testSamples*baseNames.size())
        << "%"
        << std::endl;
}

void testTrackClassification()
{
    fout.open("tracktest.log");
    if (!fout.is_open()) {
        std::cerr << "Failed to open the file" << std::endl;
        exit(1);
    }

    std::cout << "testGestureClassification, Output file: tracktest.log" << std::endl;

    std::cout << "KNN 3, size 2" << std::endl;
    testTracks<2, 3>();
    std::cout << "KNN 3, size 4" << std::endl;
    testTracks<4, 3>();
    std::cout << "KNN 3, size 6" << std::endl;
    testTracks<6, 3>();
    std::cout << "KNN 3, size 8" << std::endl;
    testTracks<8, 3>();
    std::cout << "KNN 3, size 10" << std::endl;
    testTracks<10, 3>();
    std::cout << "KNN 3, size 12" << std::endl;
    testTracks<12, 3>();
    std::cout << "KNN 3, size 14" << std::endl;
    testTracks<14, 3>();
    std::cout << "KNN 3, size 16" << std::endl;
    testTracks<16, 3>();
    std::cout << "KNN 3, size 18" << std::endl;
    testTracks<18, 3>();
    std::cout << "KNN 3, size 20" << std::endl;
    testTracks<20, 3>();

    std::cout << "KNN 5, size 2" << std::endl;
    testTracks<2, 5>();
    std::cout << "KNN 5, size 4" << std::endl;
    testTracks<4, 5>();
    std::cout << "KNN 5, size 6" << std::endl;
    testTracks<6, 5>();
    std::cout << "KNN 5, size 8" << std::endl;
    testTracks<8, 5>();
    std::cout << "KNN 5, size 10" << std::endl;
    testTracks<10, 5>();
    std::cout << "KNN 5, size 12" << std::endl;
    testTracks<12, 5>();
    std::cout << "KNN 5, size 14" << std::endl;
    testTracks<14, 5>();
    std::cout << "KNN 5, size 16" << std::endl;
    testTracks<16, 5>();
    std::cout << "KNN 5, size 18" << std::endl;
    testTracks<18, 5>();
    std::cout << "KNN 5, size 20" << std::endl;
    testTracks<20, 5>();

    std::cout << "Done!" << std::endl;
    fout.close();
}

static const std::string basegFolderLearn("gestures-learn");
static const std::string basegFolderTest("gestures-test");
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
static constexpr size_t testgSamples = 10;

bool learnGestureClassificator(BoneOrientationClassificator &classificator)
{
    for (auto it = basegNames.cbegin(); it != basegNames.cend(); ++it) {
        for (size_t i = 1; i <= learngSamples; ++i) {
            std::ostringstream file;
            file << basegFolderLearn << "/" << *it << i << ".dat";

            HandSkeleton skeleton;
            if (!loadHandSkeletonFromFile(skeleton, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            classificator.updateDatabase(*it, skeleton);
        }
    }

    return true;
}

using GestureCatalogue = NamesCatalogue<HandSkeleton>;

bool getGestureTestSamples(GestureCatalogue &catalogue)
{
    for (auto it = basegNames.cbegin(); it != basegNames.cend(); ++it) {
        for (size_t i = 1; i <= testgSamples; ++i) {
            std::ostringstream file;
            file << basegFolderTest << "/" << *it << i << ".dat";

            auto skeleton = std::make_unique<HandSkeleton>();
            if (!loadHandSkeletonFromFile(*skeleton, file.str())) {
                std::cerr << "Cannot load file " << file.str() << std::endl;
                return false;
            }

            catalogue.addElement(*it, std::move(skeleton));
        }
    }

    return true;
}

std::map<std::string, std::vector<size_t> > val;

void testGesture(int knn, float thresh)
{
    BoneOrientationClassificator classificator;
    GestureCatalogue test;

    fout << "\n\n############# Testing for knn " << knn << " and thresh " << thresh << std::endl;

    classificator.init(knn, thresh);
    if (!learnGestureClassificator(classificator)) {
        std::cerr << "Failed to learn classificator" << std::endl;
        exit(-1);
    }
    if (!getGestureTestSamples(test)) {
        std::cerr << "Failed to get test samples" << std::endl;
        exit(1);
    }

    std::map<std::string, size_t> goodMatches;

    for (auto it = test.cbegin(); it != test.cend(); ++it) {
        const std::string &myClass = it->first;
        goodMatches[myClass] = 0;
        for (auto itt = it->second.cbegin(); itt != it->second.cend(); ++itt) {
            const HandSkeleton &skeleton = **itt;
            auto desc = classificator.recognize(skeleton);
            fout
                << "Recognizing " << myClass
                << ": score " << desc.score1
                << ", class " << *desc.gestureCategory
                << ", matches " << desc.score2
                << std::endl;
            if (myClass == *desc.gestureCategory)
                ++goodMatches[myClass];
        }
    }

    size_t total = 0;
    for (auto it = goodMatches.cbegin(); it != goodMatches.cend(); ++it) {
        total += it->second;
        auto v = val.insert(std::make_pair(it->first, std::vector<size_t>()));
        v.first->second.push_back(it->second);
        fout
            << it->first/* << ": "
            << it->second << "/" << testgSamples
            << " " << it->second*100.0f/testgSamples << "%"*/
            << std::endl;
    }

    fout
        << "Total efficiency: "
        << total*100.0f/(testgSamples*basegNames.size())
        << "%"
        << std::endl;
}

void testGestureClassification()
{
    fout.open("gesturetest.log");
    if (!fout.is_open()) {
        std::cerr << "Failed to open the file" << std::endl;
        exit(1);
    }

    std::cout << "testGestureClassification, Output file: gesturetest.log" << std::endl;

    for (int k = 3; k <= 5; k += 2) {
        for (float thresh = 0; thresh <= 100.0f; thresh += 1.0f)
            testGesture(k, thresh);
    }

    std::cout << "Done!" << std::endl

    fout.close();
}

}

int main(int argc, char **argv)
{
    // Unless the ImageWindow is used, this can be commented
    //QApplication app(argc, argv);

    std::cout << "Checking pixel classification" << std::endl;
    grl::pixelClassStats();
    std::cout << "Checking track classification" << std::endl;
    grl::testTrackClassification();
    std::cout << "Checking hand gesture classification" << std::endl;
    grl::testGestureClassification();

    //return app.exec();
}
