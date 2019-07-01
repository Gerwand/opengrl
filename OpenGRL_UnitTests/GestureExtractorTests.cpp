#include "stdafx.h"
#include "CppUnitTest.h"

#include <grl/gesture/SkeletonExtractor.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenGRL_UnitTests_GestureExtractor {
TEST_CLASS(VoxelArray2DTester)
{
private:
    grl::VoxelArray2D vaImageTolerant;
    grl::VoxelArray2D vaImageUntolerant;
    grl::VoxelArray2D va;
    cv::Mat image;

    static constexpr int depthTolerance = 50;
public:
    VoxelArray2DTester()
    {
        image = cv::imread("resources/depthmap.png", cv::IMREAD_GRAYSCALE);
        Assert::IsFalse(image.empty());
        image.convertTo(image, CV_16UC1);

        Logger::WriteMessage("--In VoxelArray2DTester");
    }

    ~VoxelArray2DTester()
    {
        Logger::WriteMessage("--VoxelArray2DTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareVA)
    {
        vaImageTolerant.fromImage(image, depthTolerance);
        vaImageUntolerant.fromImage(image, 0);

        va.init(10, 15);
    }

    TEST_METHOD(initialization)
    {
        Logger::WriteMessage("----In initialization");

        // VA clear should have 8 neighbours everywhere except the corners.
        int width, height;
        va.getSize(width, height);

        int lastX = width-1;
        int lastY = height-1;
        // 3 neighbours in each conerner
        {
            Assert::AreEqual(3, va(0, 0).neighboursNumber);
            Assert::AreEqual(3, va(lastX, 0).neighboursNumber);
            Assert::AreEqual(3, va(0, lastY).neighboursNumber);
            Assert::AreEqual(3, va(lastX, lastY).neighboursNumber);
        }

        // 5 neighbours on each wall
        {
            Assert::AreEqual(5, va(0, height/2).neighboursNumber);
            Assert::AreEqual(5, va(lastX, height/2).neighboursNumber);
            Assert::AreEqual(5, va(width/2, 0).neighboursNumber);
            Assert::AreEqual(5, va(width/2, lastY).neighboursNumber);
        }

        // Image with depth tolerance set to 50 should have neighbours with 100
        // depth and 150 depth.
        {
            Assert::AreEqual(0, vaImageTolerant(0, 0).neighboursNumber);
            Assert::AreEqual(8, vaImageTolerant(2, 2).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(100, vaImageTolerant(2, 2).neighbours[0]->coords.z);
                Assert::AreEqual(150, vaImageTolerant(2, 2).neighbours[6]->coords.z);
                Assert::AreEqual(100, vaImageTolerant(2, 2).neighbours[7]->coords.z);
            }

            Assert::AreEqual(8, vaImageTolerant(2, 3).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(100, vaImageTolerant(2, 3).neighbours[0]->coords.z);
                Assert::AreEqual(150, vaImageTolerant(2, 3).neighbours[3]->coords.z);
                Assert::AreEqual(150, vaImageTolerant(2, 3).neighbours[6]->coords.z);
            }

            Assert::AreEqual(5, vaImageTolerant(2, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(150, vaImageTolerant(2, 5).neighbours[0]->coords.z);
                Assert::AreEqual(100, vaImageTolerant(2, 5).neighbours[2]->coords.z);
                Assert::AreEqual(150, vaImageTolerant(2, 5).neighbours[3]->coords.z);
                Assert::AreEqual(100, vaImageTolerant(2, 5).neighbours[4]->coords.z);
            }

            Assert::AreEqual(3, vaImageTolerant(5, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(100, vaImageTolerant(5, 5).neighbours[0]->coords.z);
                Assert::AreEqual(100, vaImageTolerant(5, 5).neighbours[1]->coords.z);
                Assert::AreEqual(100, vaImageTolerant(5, 5).neighbours[2]->coords.z);
            }

            Assert::AreEqual(5, vaImageTolerant(6, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(250, vaImageTolerant(6, 5).neighbours[0]->coords.z);
                Assert::AreEqual(250, vaImageTolerant(6, 5).neighbours[3]->coords.z);
                Assert::AreEqual(250, vaImageTolerant(6, 5).neighbours[4]->coords.z);
            }

            Assert::AreEqual(2, vaImageTolerant(5, 6).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(0, vaImageTolerant(5, 6).neighbours[0]->coords.z);
                Assert::AreEqual(0, vaImageTolerant(5, 6).neighbours[1]->coords.z);
            }
        }

        // No tolerance should have only neighbours which have the same depth
        {
            Assert::AreEqual(0, vaImageUntolerant(0, 0).neighboursNumber);
            Assert::AreEqual(4, vaImageUntolerant(2, 2).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(100, vaImageUntolerant(2, 2).neighbours[0]->coords.z);
                Assert::AreEqual(100, vaImageUntolerant(2, 2).neighbours[2]->coords.z);
                Assert::AreEqual(100, vaImageUntolerant(2, 2).neighbours[3]->coords.z);
            }

            Assert::AreEqual(4, vaImageUntolerant(2, 3).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(150, vaImageUntolerant(2, 3).neighbours[0]->coords.z);
                Assert::AreEqual(150, vaImageUntolerant(2, 3).neighbours[2]->coords.z);
                Assert::AreEqual(150, vaImageUntolerant(2, 3).neighbours[3]->coords.z);
            }

            Assert::AreEqual(3, vaImageUntolerant(2, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(150, vaImageUntolerant(2, 5).neighbours[0]->coords.z);
                Assert::AreEqual(150, vaImageUntolerant(2, 5).neighbours[1]->coords.z);
                Assert::AreEqual(150, vaImageUntolerant(2, 5).neighbours[2]->coords.z);
            }

            Assert::AreEqual(3, vaImageUntolerant(5, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(100, vaImageUntolerant(5, 5).neighbours[0]->coords.z);
                Assert::AreEqual(100, vaImageUntolerant(5, 5).neighbours[1]->coords.z);
                Assert::AreEqual(100, vaImageUntolerant(5, 5).neighbours[2]->coords.z);
            }

            Assert::AreEqual(5, vaImageUntolerant(6, 5).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(250, vaImageUntolerant(6, 5).neighbours[0]->coords.z);
                Assert::AreEqual(250, vaImageUntolerant(6, 5).neighbours[3]->coords.z);
                Assert::AreEqual(250, vaImageUntolerant(6, 5).neighbours[4]->coords.z);
            }

            Assert::AreEqual(2, vaImageUntolerant(5, 6).neighboursNumber);
            // Check depths of some neighbours
            {
                Assert::AreEqual(0, vaImageUntolerant(5, 6).neighbours[0]->coords.z);
                Assert::AreEqual(0, vaImageUntolerant(5, 6).neighbours[1]->coords.z);
            }
        }

        Logger::WriteMessage("----initialization Done");
    }

    TEST_METHOD(getters)
    {
        Logger::WriteMessage("----In getters");

        // Check if the depth and the coords are the same as expected (for loop)
        int width, height;
        vaImageTolerant.getSize(width, height);
        Assert::AreEqual(image.cols, width);
        Assert::AreEqual(image.rows, height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cv::Point coords(x, y);
                int expected = image.at<uint16_t>(coords);
                int value = vaImageTolerant.getVoxelValue(x, y);
                Assert::AreEqual(expected, value);

                grl::FFVoxel &voxel = vaImageTolerant.getVoxel(x, y);
                Assert::AreEqual(x, voxel.coords.x);
                Assert::AreEqual(y, voxel.coords.y);
                Assert::AreEqual(expected, voxel.coords.z);
            }
        }

        va.getSize(width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                grl::FFVoxel &voxel = va.getVoxel(x, y);
                Assert::AreEqual(x, voxel.coords.x);
                Assert::AreEqual(y, voxel.coords.y);
                Assert::AreEqual(0, voxel.coords.z);
            }
        }

        Logger::WriteMessage("----getters Done");
    }

    TEST_METHOD(setters)
    {
        Logger::WriteMessage("----In setters");

        // Set some values and then check if they changed properly.
        va.setVoxelValue(10, 5, 30);
        Assert::AreEqual(30, va.getVoxelValue(10, 5));
        Assert::AreEqual(30, va.getVoxel(10, 5).coords.z);

        Logger::WriteMessage("----setters Done");
    }
};

TEST_CLASS(FloodFillClippedTester)
{
private:
    grl::FloodFillClipped ffTolerant;
    grl::FloodFillClipped ffUntolerant;
    cv::Mat image;

    static constexpr int depthTolerance = 50;
public:
    FloodFillClippedTester()
    {
        image = cv::imread("resources/depthmap.png", cv::IMREAD_GRAYSCALE);
        Assert::IsFalse(image.empty());
        image.convertTo(image, CV_16UC1);

        Logger::WriteMessage("--In FloodFillClippedTester");
    }

    ~FloodFillClippedTester()
    {
        Logger::WriteMessage("--FloodFillClippedTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareFloodFill)
    {
        ffTolerant.init(depthTolerance, image);
        ffUntolerant.init(0, image);
    }

    TEST_METHOD(multipleExtraction)
    {
        grl::Plane notClippingPlane(grl::Vec3f(0.0f, 0.0f, 1.0f), grl::Vec3f(0.0f, 0.0f, 0.0f));
        grl::DepthObject object;

        // Check if the same object will be extracted twice
        Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(0, 0), notClippingPlane, object));
        Assert::AreEqual(static_cast<size_t>(1), object.getSize());
        Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(0, 0), notClippingPlane, object));

        // Check if the second object is extracted properly after the first one is extracted
        {
            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(2, 3), notClippingPlane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(35), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            cv::Mat roi = image(object.getBoundingBox());
            cv::Mat depth = object.getDepthImageOfObject();
            for (int y = 0; y < depth.rows; ++y) {
                for (int x = 0; x < depth.cols; ++x) {
                    int expected = roi.at<uint16_t>(y, x);
                    int value = depth.at<uint16_t>(y, x);
                    Assert::AreEqual(expected, value);
                }
            }
        }

        // And another object
        {
            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(6, 0), notClippingPlane, object));
            Assert::AreEqual(3, object.getBoundingBox().width);
            Assert::AreEqual(9, object.getBoundingBox().height);
            Assert::AreEqual(6, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(27), object.getSize());
            Assert::AreEqual(250, object.getMaxDepthValue());
            Assert::AreEqual(250, object.getMinDepthValue());

            cv::Mat roi = image(object.getBoundingBox());
            cv::Mat depth = object.getDepthImageOfObject();
            for (int y = 0; y < depth.rows; ++y) {
                for (int x = 0; x < depth.cols; ++x) {
                    int expected = roi.at<uint16_t>(y, x);
                    int value = depth.at<uint16_t>(y, x);
                    Assert::AreEqual(expected, value);
                }
            }
        }

        // And the last one
        {
            Assert::IsTrue(ffUntolerant.extractObject(grl::Vec2i(2, 2), notClippingPlane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(17), object.getSize());
            Assert::AreEqual(100, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());
        }
    }

    TEST_METHOD(clippedExtractionNegativeX)
    {
        grl::DepthObject object;

        grl::Vec3f normal(-1.0f, 0.0f, 0.0f);

        {
            grl::Plane plane(normal, grl::Vec3f(2.0f, 0.0f, 0.0f));

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(3, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(17), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsTrue(ffUntolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(3, object.getBoundingBox().width);
            Assert::AreEqual(3, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(8), object.getSize());
            Assert::AreEqual(100, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(3, 2), plane, object));
            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(6, 0), plane, object));
            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(0, 7), plane, object));
            Assert::AreEqual(3, object.getBoundingBox().width);
            Assert::AreEqual(3, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(7, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(9), object.getSize());
            Assert::AreEqual(200, object.getMaxDepthValue());
            Assert::AreEqual(200, object.getMinDepthValue());
        }

        {
            grl::Plane plane(normal, grl::Vec3f(7.0f, 5.0f, -2.0f));

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(35), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsTrue(ffUntolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(17), object.getSize());
            Assert::AreEqual(100, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(6, 0), plane, object));
            Assert::AreEqual(2, object.getBoundingBox().width);
            Assert::AreEqual(9, object.getBoundingBox().height);
            Assert::AreEqual(6, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(18), object.getSize());
            Assert::AreEqual(250, object.getMaxDepthValue());
            Assert::AreEqual(250, object.getMinDepthValue());
        }
    }

    TEST_METHOD(clippedExtractionPositiveX)
    {
        grl::DepthObject object;

        grl::Vec3f normal(1.0f, 0.0f, 0.0f);

        {
            grl::Plane plane(normal, grl::Vec3f(2.0f, 0.0f, 0.0f));

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(4, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(2, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(24), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsTrue(ffUntolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(4, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(2, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(12), object.getSize());
            Assert::AreEqual(100, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());

            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(1, 2), plane, object));
            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(6, 0), plane, object));
            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(0, 7), plane, object));
        }
    }

    TEST_METHOD(clippedExtractionNegativeY)
    {
        // Check if the second object is extracted properly after the first one is extracted
        grl::DepthObject object;

        grl::Vec3f normal(0.0f, -1.0f, 0.0f);

        {
            grl::Plane plane(normal, grl::Vec3f(0.0f, 2.0f, 0.0f));

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(2, 2), plane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(3, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(17), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(100, object.getMinDepthValue());
            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(2, 3), plane, object));
        }
    }

    TEST_METHOD(clippedExtractionPositiveZ)
    {
        // Check if the second object is extracted properly after the first one is extracted
        grl::DepthObject object;

        grl::Vec3f normal(0.0f, 0.0f, 1.0f);

        {
            grl::Plane plane(normal, grl::Vec3f(0.0f, 0.0f, 150.0f));

            Assert::IsTrue(ffTolerant.extractObject(grl::Vec2i(3, 2), plane, object));
            Assert::AreEqual(6, object.getBoundingBox().width);
            Assert::AreEqual(6, object.getBoundingBox().height);
            Assert::AreEqual(0, object.getBoundingBox().x);
            Assert::AreEqual(0, object.getBoundingBox().y);
            Assert::AreEqual(static_cast<size_t>(18), object.getSize());
            Assert::AreEqual(150, object.getMaxDepthValue());
            Assert::AreEqual(150, object.getMinDepthValue());
            Assert::IsFalse(ffTolerant.extractObject(grl::Vec2i(2, 2), plane, object));
        }
    }
};


TEST_CLASS(SkeletonExtractorTester)
{
private:
    grl::SkeletonExtractor extractor;
    grl::Skeleton skeleton;
    cv::Mat image;

    static constexpr int depthTolerance = 50;
public:
    SkeletonExtractorTester()
    {
        image = cv::imread("resources/depthmap.png", cv::IMREAD_GRAYSCALE);
        Assert::IsFalse(image.empty());
        image.convertTo(image, CV_16UC1);

        Logger::WriteMessage("--In SkeletonExtractorTester");
    }

    ~SkeletonExtractorTester()
    {
        Logger::WriteMessage("--SkeletonExtractorTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareSkeletonExtractor)
    {
        grl::SkeletonExtractorConfig config;
        config.depthTolerance = depthTolerance;

        extractor.init(config);

        grl::Joint &lw = skeleton.joints[grl::LEFT_WRIST];
        lw.tracked = true;
        lw.coordDepthImage = grl::Vec2i(0, 8);

        grl::Joint &le = skeleton.joints[grl::LEFT_ELBOW];
        le.tracked = true;
        le.coordDepthImage = grl::Vec2i(4, 8);

        grl::Joint &rw = skeleton.joints[grl::RIGHT_WRIST];
        rw.tracked = true;
        rw.coordDepthImage = grl::Vec2i(7, 0);
        grl::Joint &re = skeleton.joints[grl::RIGHT_ELBOW];
        re.tracked = true;
        re.coordDepthImage = grl::Vec2i(7, 8);
    }

    TEST_METHOD(extract)
    {
        grl::DepthObject leftHand, rightHand;

        extractor.extractHands(image, skeleton, leftHand, rightHand);

        // Left hand verification
        Assert::AreEqual(std::numeric_limits<uint8_t>::max(), leftHand.getAccuracy());
        Assert::AreEqual(3, leftHand.getBoundingBox().width);
        Assert::AreEqual(3, leftHand.getBoundingBox().height);
        Assert::AreEqual(0, leftHand.getBoundingBox().x);
        Assert::AreEqual(7, leftHand .getBoundingBox().y);
        Assert::AreEqual(static_cast<size_t>(9), leftHand.getSize());
        Assert::AreEqual(200, leftHand.getMaxDepthValue());
        Assert::AreEqual(200, leftHand.getMinDepthValue());

        // Left hand verification
        Assert::AreEqual(std::numeric_limits<uint8_t>::max(), rightHand.getAccuracy());
        Assert::AreEqual(3, rightHand.getBoundingBox().width);
        Assert::AreEqual(5, rightHand.getBoundingBox().height);
        Assert::AreEqual(6, rightHand.getBoundingBox().x);
        Assert::AreEqual(0, rightHand.getBoundingBox().y);
        Assert::AreEqual(static_cast<size_t>(15), rightHand.getSize());
        Assert::AreEqual(250, rightHand.getMaxDepthValue());
        Assert::AreEqual(250, rightHand.getMinDepthValue());
    }
};

}
