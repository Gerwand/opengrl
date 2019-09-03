#include "ImageWindow.h"
#include <grl/gesture/RDFHandSkeletonExtractor.h>
#include <grl/gesture/GestureClassificator.h>
#include <grl/rdf/RDFUtils.h>
#include <grl/camera/KinectCamera.h>
#include <grl/track/TrackClassificator.h>

namespace grl {

void drawJointLines(HandSkeleton &skeleton, cv::Mat &image)
{
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        const HandJoint &joint = skeleton[i];

        if (joint.certainty <= 0.01f)
            continue;

        std::vector<HandJoint *> children;
        skeleton.getAllChildren(joint.type, children);

        for (auto itc = children.cbegin(); itc != children.cend(); ++itc) {
            HandJoint &jointChild = **itc;
            if (jointChild.certainty <= 0.01f)
                continue;

            cv::line(image,
                     cv::Point(joint.locationImage.x, joint.locationImage.y),
                     cv::Point(jointChild.locationImage.x, jointChild.locationImage.y),
                     cv::Scalar(125, 0, 125), 1);
        }
    }
}

void drawJointPoints(HandSkeleton &skeleton, cv::Mat &image)
{
    for (size_t i = 0; i < grlHandJointNum; ++i) {
        const HandJoint &joint = skeleton[i];

        if (joint.certainty <= 0.01f)
            continue;
        cv::circle(image, cv::Point(joint.locationImage.x, joint.locationImage.y), 1, cv::Scalar(255, 255, 255), 2);
    }
}

void drawJointsOnImage(HandSkeleton &skeleton, cv::Mat &image)
{
    drawJointLines(skeleton, image);
    drawJointPoints(skeleton, image);
}

std::shared_ptr< std::vector<Voxel> > convertDepthToVoxels(cv::Mat &image)
{
    std::shared_ptr< std::vector<Voxel> > voxels(new std::vector<Voxel>);

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            Voxel v;
            v.coords.z = std::round(image.at<float>(cv::Point(x, y))*1000);
            // Don't convert background as it makes no use for the depth object.
            if (v.coords.z <= 0 || v.coords.z >= 8000)
                continue;
            v.coords.x = x;
            v.coords.y = y;
            voxels->push_back(v);
        }
    }

    return voxels;
}

std::shared_ptr< std::vector<Voxel> > convertDepthToObject(cv::Mat &image, DepthObject &object)
{
    auto voxels = convertDepthToVoxels(image);

    for (auto it = voxels->begin(); it != voxels->end(); ++it) {
        object.putVoxel(&(*it));
    }

    return voxels;
}

void showSampleRDF()
{
    const char *className = "E:\\Magisterka\\generated-train-small\\hand_classes_0008889.png";
    const char *depthName = "E:\\Magisterka\\generated-train-small\\hand_depth_0008889.exr";
    cv::Mat classImage, depthImage;
    grl::RDFTools::loadDepthImageWithClasses(className, depthName, classImage, depthImage);

    cv::Mat classImageRGB;
    grl::RDFTools::convertHandClassesToRGB(classImage, classImageRGB);
    static ImageWindow imgw1(classImageRGB);

    grl::DepthObject depthObject;
    auto voxels = grl::convertDepthToObject(depthImage, depthObject);

    grl::RDFHandSkeletonExtractor extractor;
    if (!extractor.init("E:\\Magisterka\\OpenGRL\\resources\\rdf.txt")) {
        std::cout << "Invalid forest" << std::endl;
        exit(1);
    }

    grl::HandSkeleton handSkeleton;
    extractor.extractSkeleton(depthObject, handSkeleton);

    cv::Mat calculatedClassesRGB;
    cv::Mat assignedClasses = extractor.getLastClasses();
    grl::RDFTools::convertHandClassesToRGB(assignedClasses, calculatedClassesRGB);
    //grl::drawJointsOnImage(handSkeleton, calculatedClassesRGB);
    static ImageWindow imgw2(calculatedClassesRGB);

    imgw1.show();
    imgw2.show();

    BoneOrientationClassificator knn;
    knn.init(5, 0.01f);

    BoneOrientationClassificator::Features features;
    knn.extractFeatures(handSkeleton, features);

    int i = 0;
    for (auto it = features.cbegin(); it != features.cend(); ++it, ++i) {
        std::cout << "Bone: " << i << " , feature: " << it->get() << std::endl;
    }
}


void getKinectParameters()
{
    KinectCamera kinect;

    if (kinect.init() != 0) {
        std::cout << "Error, kinect not available\n";
        exit(1);
    }

    kinect.dumpStats();
}

void pixelClassStats()
{
    grl::RandomDecisionForest forest;
    if (!forest.loadFromFile("E:\\Magisterka\\OpenGRL\\resources\\rdf.txt")) {
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
            "E:\\Magisterka\\generated-train-small\\hand_classes_", // png
            "E:\\Magisterka\\generated-train-small\\hand_depth_", // png
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
    fout.open("trackclass.log");
    if (!fout.is_open()) {
        std::cerr << "Failed to open the file" << std::endl;
        exit(1);
    }

    std::cout << "KNN 3, size 2" << std::endl;
    testTracks<2, 3>();
    std::cout << "KNN 3, size 3" << std::endl;
    testTracks<4, 3>();
    std::cout << "KNN 3, size 4" << std::endl;
    testTracks<6, 3>();
    std::cout << "KNN 3, size 5" << std::endl;
    testTracks<8, 3>();
    std::cout << "KNN 3, size 6" << std::endl;
    testTracks<10, 3>();
    std::cout << "KNN 3, size 7" << std::endl;
    testTracks<12, 3>();
    std::cout << "KNN 3, size 8" << std::endl;
    testTracks<14, 3>();
    std::cout << "KNN 3, size 9" << std::endl;
    testTracks<16, 3>();
    std::cout << "KNN 3, size 10" << std::endl;
    testTracks<18, 3>();
    std::cout << "KNN 3, size 20" << std::endl;
    testTracks<20, 3>();

    std::cout << "KNN 5, size 2" << std::endl;
    testTracks<2, 5>();
    std::cout << "KNN 5, size 3" << std::endl;
    testTracks<4, 5>();
    std::cout << "KNN 5, size 4" << std::endl;
    testTracks<6, 5>();
    std::cout << "KNN 5, size 5" << std::endl;
    testTracks<8, 5>();
    std::cout << "KNN 5, size 6" << std::endl;
    testTracks<10, 5>();
    std::cout << "KNN 5, size 7" << std::endl;
    testTracks<12, 5>();
    std::cout << "KNN 5, size 8" << std::endl;
    testTracks<14, 5>();
    std::cout << "KNN 5, size 9" << std::endl;
    testTracks<16, 5>();
    std::cout << "KNN 5, size 10" << std::endl;
    testTracks<18, 5>();
    std::cout << "KNN 5, size 20" << std::endl;
    testTracks<20, 5>();

    std::cout << "Done!" << std::endl;
    exit(0);
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


    for (int k = 3; k <= 5; k += 2) {
        for (float thresh = 0; thresh <= 100.0f; thresh += 1.0f)
            testGesture(k, thresh);
        //fout << " k = " << k << std::endl;

        //for (auto it = val.begin(); it != val.end(); ++it) {
        //    fout << it->first << std::endl;
        //    for (auto t = it->second.begin(); t != it->second.end(); ++t) {
        //        fout << *t << std::endl;
        //    }
        //}
        //val.clear();
    }

    exit(0);
}

}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    //grl::showSampleRDF();
    //grl::getKinectParameters();
    //grl::pixelClassStats();
    //grl::testTrackClassification();
    grl::testGestureClassification();

    return app.exec();
}
