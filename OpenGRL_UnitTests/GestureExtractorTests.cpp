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

        // Checking random voxels if they are not analyzed
        {
            Assert::IsFalse(va(0, 0).analyzed);
            Assert::IsFalse(va(3, 5).analyzed);
            Assert::IsFalse(va(2, 12).analyzed);
            Assert::IsFalse(va(8, 10).analyzed);
            Assert::IsFalse(va(5, 1).analyzed);
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

}
