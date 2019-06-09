#include "stdafx.h"
#include "CppUnitTest.h"

#include <grl/utils/MathUtils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_MODULE_INITIALIZE(ModuleInitialize)
{
    Logger::WriteMessage("Running MathUtils tests");
}

TEST_MODULE_CLEANUP(ModuleCleanup)
{
    Logger::WriteMessage("MathUtils tests done");
}

namespace OpenGRL_UnitTests_Math
{
TEST_CLASS(MathFunctionsTester)
{
private:
    template<typename T>
    struct MinMax
    {
        T min;
        T max;
    };

    static constexpr float floatTolerance = 0.00001f;
    static constexpr size_t msgBufSize = 256;

    wchar_t message[msgBufSize];
public:
    MathFunctionsTester()
    {
        Logger::WriteMessage("--In MathFunctionsTester");
    }

    ~MathFunctionsTester()
    {
        Logger::WriteMessage("--MathFunctionsTester Done");
    }

    TEST_METHOD(asbBetweenInt)
    {
        Logger::WriteMessage("----In asbBetweenInt");

        constexpr size_t valuesSize = 3;
        int values[valuesSize] = { -5, 10, 0 };

        constexpr size_t examplesSize = 3;
        int trueRanges[valuesSize][examplesSize] = {
            { 5, 6, 10 },
            { 10, 11, 15 },
            { 1, 2, 3 }
        };
        // 3rd value does not have false scenario
        int falseRanges[valuesSize-1][examplesSize] = {
            { 4, 3, 1 },
            { 9, 8, 1 },
        };

        // True scenarios
        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %d and comp: %d. Should be true (val in range), was false (val not in range).",
                              values[n], trueRanges[n][i]);
                Assert::IsTrue(grl::absBetween(values[n], trueRanges[n][i]), message);
            }
        }

        // False scenarios
        for (size_t n = 0; n < valuesSize-1; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Succeed for val: %d and comp: %d. Should be false (val not in range), was true (val in range).",
                              values[n], falseRanges[n][i]);
                Assert::IsFalse(grl::absBetween(values[n], falseRanges[n][i]), message);
            }
        }

        Logger::WriteMessage("----asbBetweenInt done");
    }

    TEST_METHOD(asbBetweenFloat)
    {
        Logger::WriteMessage("----In asbBetweenFloat");

        constexpr size_t valuesSize = 4;
        float values[valuesSize] = { -4.9f, 10.321f, 2.0f, 0.0f };

        constexpr size_t examplesSize = 4;
        float trueRanges[valuesSize][examplesSize] = {
            { 4.9f, 4.901f, 5.0f, 6.0f},
            { 10.321f, 10.322f, 11.0f, 15.3f},
            { 2.0f, 2.001f, 2.1f, 3.0f },
            { 0.001f, 0.1f, 1.0f, 8.2f },
        };
        // 4th value does not have false scenario
        float falseRanges[valuesSize-1][examplesSize] = {
            { 4.899f, 4.8f, 4.0f, 3.0f },
            { 10.3209f, 10.32f, 10.0f, 2.0f },
            { 1.999f, 1.9f, 1.0f, 0.5f },
        };

        // True scenarios
        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %f and comp: %f. Should be true (val in range), was false (val not in range).",
                              values[n], trueRanges[n][i]);
                Assert::IsTrue(grl::absBetween(values[n], trueRanges[n][i]), message);
            }
        }

        // False scenarios
        for (size_t n = 0; n < valuesSize-1; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Succeed for val: %f and comp: %f. Should be false (val not in range), was true (val in range).",
                              values[n], falseRanges[n][i]);
                Assert::IsFalse(grl::absBetween(values[n], falseRanges[n][i]), message);
            }
        }

        Logger::WriteMessage("----asbBetweenFloat done");
    }

    TEST_METHOD(isBetweenInt)
    {
        Logger::WriteMessage("----In isBetweenInt");

        constexpr size_t valuesSize = 3;
        int values[valuesSize] = { -5, 10, 0 };

        constexpr size_t examplesSize = 4;
        MinMax<int> trueRanges[valuesSize][examplesSize] = {
            { {-6,-5}, {-5,-4}, {-6,-4}, {-10, 10}},
            { {10,11}, {9,10}, {9,11}, {-15, 15}},
            { {0,1}, {-1,0}, {-1,1}, {-5, 5} }
        };
        MinMax<int> falseRanges[valuesSize][examplesSize] = {
            { {-4,-3}, {-7,-6}, {4,6}, {-1,11} },
            { {8,9}, {11,12}, {-11,-9}, {-15,5} },
            { {-2,-1}, {1,2}, {3,15}, {-15,-3} },
        };

        // True scenarios
        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %d and range {%d,%d}. Should be true (val in range), was false (val not in range).",
                              values[n], trueRanges[n][i].min, trueRanges[n][i].max);
                Assert::IsTrue(grl::isBetween(values[n], trueRanges[n][i].max, trueRanges[n][i].min), message);
            }
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %d and range {%d,%d}. Should be false (val not in range), was true (val in range).",
                              values[n], falseRanges[n][i].min, falseRanges[n][i].max);
                Assert::IsFalse(grl::isBetween(values[n], falseRanges[n][i].max, falseRanges[n][i].min), message);
            }
        }

        Logger::WriteMessage("----isBetweenInt done");
    }

    TEST_METHOD(isBetweenFloat)
    {
        Logger::WriteMessage("----In isBetweenFloat");

        constexpr size_t valuesSize = 3;
        float values[valuesSize] = { -4.9f, 10.321f, 0.0f };

        constexpr size_t examplesSize = 4;
        MinMax<float> trueRanges[valuesSize][examplesSize] = {
            { {-4.91f,-4.9f}, {-4.9f,-4.89f}, {-4.91f,-4.89f}, {-8.0f, 5.0f}},
            { {10.321f,10.322f}, {10.320f,10.321f}, {10.320f,10.322f}, {-9.0f, 20.0f}},
            { {-0.01f,0.0f}, {0.0f,0.01f}, {-0.01f,0.01f}, {-5.0f, 5.0f} }
        };
        MinMax<float> falseRanges[valuesSize][examplesSize] = {
            { {-4.92f,-4.91f}, {-4.89f,-4.88f}, {-11.0f,-5.0f}, {-2.0f,10.0f} },
            { {10.322f,10.323f}, {10.319f,10.320f}, {11.0f,15.0f}, {-15.0f,6.0f} },
            { {-0.02f,-0.01f}, {0.01f,0.02f}, {11.0f,15.0f}, {-15.0f,-6.0f} },
        };

        // True scenarios
        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %f and range {%f,%f}. Should be true (val in range), was false (val not in range).",
                              values[n], trueRanges[n][i].min, trueRanges[n][i].max);
                Assert::IsTrue(grl::isBetween(values[n], trueRanges[n][i].max, trueRanges[n][i].min), message);
            }
            for (size_t i = 0; i < examplesSize; ++i) {
                std::swprintf(message, msgBufSize,
                              L"Failed for val: %f and range {%f,%f}. Should be false (val not in range), was true (val in range).",
                              values[n], falseRanges[n][i].min, falseRanges[n][i].max);
                Assert::IsFalse(grl::isBetween(values[n], falseRanges[n][i].max, falseRanges[n][i].min), message);
            }
        }

        Logger::WriteMessage("----isBetweenFloat done");
    }

    TEST_METHOD(clampInt)
    {
        Logger::WriteMessage("----In clampInt");

        constexpr size_t valuesSize = 2;
        int values[valuesSize] = { 5, -5 };

        constexpr size_t examplesSize = 7;
        MinMax<int> ranges[valuesSize][examplesSize] = {
            { {-1,5}, {1,4}, {1,6}, {5,10}, {6,10}, {4,10}, {-1, 11} },
            { {-9,-5}, {-9,-6}, {-9,-4}, {-4,4}, {-5,4}, {-6,7}, {-9,1} },
        };
        int expectedValues[valuesSize][examplesSize] = {
            {5, 4, 5, 5, 6, 5, 5},
            {-5, -6, -5, -4, -5, -5, -5},
        };

        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                MinMax<int> &range = ranges[n][i];
                int result = grl::clamp(values[n], range.min, range.max);
                swprintf(message, msgBufSize,
                         L"Clamping %d to range (%d, %d).\n",
                         values[n], range.min, range.max);
                Assert::AreEqual(expectedValues[n][i], result, message);
            }
        }

        Logger::WriteMessage("----clampInt done");
    }

    TEST_METHOD(clampFloat)
    {
        Logger::WriteMessage("----In clampFloat");
        constexpr size_t valuesSize = 2;
        float values[valuesSize] = { 5.2f, -1.53f };

        constexpr size_t examplesSize = 7;
        MinMax<float> ranges[valuesSize][examplesSize] = {
            { {0.0f,5.2f}, {1.0f,5.21f}, {0.0f,5.19f},
              {5.2f,10.0f}, {5.21f,10.0f}, {5.19f,10.0f},
              {-1.0f, 11.0f} },
            { {-9.0f,-1.53f}, {-9.0f,-1.531f}, {-9.0f,-1.529f},
              {-1.53f,4.0f}, {-1.531f,4.0f}, {-1.529f,4.0f},
              {-9.2f,1.5f} },
        };
        float expectedValues[valuesSize][examplesSize] = {
            {5.2f, 5.2f, 5.19f, 5.2f, 5.21f, 5.2f, 5.2f},
            {-1.53f, -1.531f, -1.53f, -1.53f, -1.53f, -1.529f, -1.53f},
        };

        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                MinMax<float> &range = ranges[n][i];
                float result = grl::clamp(values[n], range.min, range.max);
                swprintf(message, msgBufSize,
                         L"Clamping %f to range (%f, %f)\n",
                         values[n], range.min, range.max);
                Assert::AreEqual(expectedValues[n][i], result, floatTolerance, message);
            }
        }

        Logger::WriteMessage("----clampFloat done");
    }

    TEST_METHOD(clampMin)
    {
        Logger::WriteMessage("----In clampMin");

        constexpr size_t valuesSize = 2;
        int values[valuesSize] = { 5, -5 };

        constexpr size_t examplesSize = 4;
        int minimums[valuesSize][examplesSize] = {
            { 4, 5, 6, 10 },
            { -6, -5, -4, 1 },
        };
        int expectedValues[valuesSize][examplesSize] = {
            { 5, 5, 6, 10 },
            { -5, -5, -4, 1 }
        };

        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                int result = grl::clampMin(values[n], minimums[n][i]);
                swprintf(message, msgBufSize,
                         L"Clamping %d to range (%d, -).\n",
                         values[n], minimums[n][i]);
                Assert::AreEqual(expectedValues[n][i], result, message);
            }
        }

        Logger::WriteMessage("----clampMin done");
    }

    TEST_METHOD(clampMax)
    {
        Logger::WriteMessage("----In clampMax");

        constexpr size_t valuesSize = 2;
        int values[valuesSize] = { 5, -5 };

        constexpr size_t examplesSize = 4;
        int maximums[valuesSize][examplesSize] = {
            { 4, 5, 6, 1 },
            { -6, -5, -4, -10 },
        };
        int expectedValues[valuesSize][examplesSize] = {
            { 4, 5, 5, 1 },
            { -6, -5, -5, -10 },
        };

        for (size_t n = 0; n < valuesSize; ++n) {
            for (size_t i = 0; i < examplesSize; ++i) {
                int result = grl::clampMax(values[n], maximums[n][i]);
                swprintf(message, msgBufSize,
                         L"Clamping %d to range (-, %d).\n",
                         values[n], maximums[n][i]);
                Assert::AreEqual(expectedValues[n][i], result, message);
            }
        }

        Logger::WriteMessage("----clampMax done");
    }

    TEST_METHOD(sigmaInt)
    {
        Logger::WriteMessage("----In sigmaInt");

        constexpr size_t valuesSize = 5;
        int values[valuesSize] = { 12, -12, -1, 1, 0 };
        int expectedValues[valuesSize] = { 1, -1, -1, 1, 1 };

        for (size_t n = 0; n < valuesSize; ++n) {
            int result = grl::sigma(values[n]);
            swprintf(message, msgBufSize,
                     L"Sigmoid function for input %d.\n", values[n]);
            Assert::AreEqual(expectedValues[n], result, message);
        }

        Logger::WriteMessage("----sigmaInt done");
    }

    TEST_METHOD(sigmaFloat)
    {
        Logger::WriteMessage("----In sigmaFloat");

        constexpr size_t valuesSize = 5;
        float values[valuesSize] = { 6.4f, -2.5f, -0.001f, 0.001f, 0.0f };
        int expectedValues[valuesSize] = { 1, -1, -1, 1, 1 };

        for (size_t n = 0; n < valuesSize; ++n) {
            int result = grl::sigma(values[n]);
            swprintf(message, msgBufSize,
                     L"Sigmoid function for input %f.\n", values[n]);
            Assert::AreEqual(expectedValues[n], result, message);
        }

        Logger::WriteMessage("----sigmaFloat done");
    }
};

TEST_CLASS(Vector2Tester)
{
private:
    static constexpr float floatTolerance = 0.001f;
public:
    Vector2Tester()
    {
        Logger::WriteMessage("--In Vector2Tester");
    }

    ~Vector2Tester()
    {
        Logger::WriteMessage("--Vector2Tester Done");
    }

    TEST_METHOD(lengthInt)
    {
        Logger::WriteMessage("----In lengthInt");

        grl::Vec2i vec = { 1, 0 };
        Assert::AreEqual(1.0f, vec.length());

        vec = { 0, 1 };
        Assert::AreEqual(1.0f, vec.length());

        vec.x = 2; vec.y = 5;
        Assert::AreEqual(5.385f, vec.length(), floatTolerance);

        Logger::WriteMessage("----lengthInt Done");
    }

    TEST_METHOD(lengthFloat)
    {
        Logger::WriteMessage("----In lengthFloat");

        grl::Vec2f vec = { 1.0f, 0.0f };
        Assert::AreEqual(1.0f, vec.length());

        vec = { 0.0f, 1.0f };
        Assert::AreEqual(1.0f, vec.length());

        vec.x = 2.5f; vec.y = 5.2f;
        Assert::AreEqual(5.769f, vec.length(), floatTolerance);

        Logger::WriteMessage("----lengthFloat Done");
    }


    TEST_METHOD(toCVPointInt)
    {
        Logger::WriteMessage("----In toCPPointInt");
        cv::Point p;
        grl::Vec2i vec = { 3, 5 };

        p = vec;
        Assert::AreEqual(p.x, vec.x);
        Assert::AreEqual(p.y, vec.y);

        Logger::WriteMessage("----toCVPointDone Done");
    }
};

TEST_CLASS(Vector3Tester)
{
private:
    static constexpr float floatTolerance = 0.001f;

    grl::Vec3f vec1;
    grl::Vec3f vec2;
public:
    Vector3Tester()
    {
        Logger::WriteMessage("--In Vector3Tester");
    }

    ~Vector3Tester()
    {
        Logger::WriteMessage("--Vector3Tester Done");
    }

    TEST_METHOD_INITIALIZE(prepareVectors)
    {
        vec1 = grl::Vec3f(1.0f, 3.0f, -2.0f);
        vec2 = grl::Vec3f(5.0f, -3.0f, 2.0f);
    }

    TEST_METHOD(dot)
    {
        Logger::WriteMessage("----In dot");

        float dotProduct = vec1.dot(vec2);
        Assert::AreEqual(-8.0f, dotProduct, floatTolerance);

        Logger::WriteMessage("----dot Done");
    }

    TEST_METHOD(cross)
    {
        Logger::WriteMessage("----In dot");

        grl::Vec3f ortho = vec1.cross(vec2);
        Assert::AreEqual(0.0f, ortho.x, floatTolerance);
        Assert::AreEqual(-12.0f, ortho.y, floatTolerance);
        Assert::AreEqual(-18.0f, ortho.z, floatTolerance);

        Logger::WriteMessage("----dot Done");
    }

    TEST_METHOD(length)
    {
        Logger::WriteMessage("----In length");

        Assert::AreEqual(1.0f, grl::Vec3f(1.0f, 0.0f, 0.0f).length());
        Assert::AreEqual(1.0f, grl::Vec3f(0.0f, 1.0f, 0.0f).length());
        Assert::AreEqual(1.0f, grl::Vec3f(0.0f, 0.0f, -1.0f).length());
        Assert::AreEqual(3.741f, vec1.length(), floatTolerance);
        Assert::AreEqual(6.164f, vec2.length(), floatTolerance);

        Logger::WriteMessage("----length Done");
    }

    TEST_METHOD(normalize)
    {
        Logger::WriteMessage("----In normalize");

        grl::Vec3f unit(5.0f, 0.0f, 0.0f);
        unit.normalize();
        Assert::AreEqual(1.0f, unit.x);
        Assert::AreEqual(0.0f, unit.y);
        Assert::AreEqual(0.0f, unit.z);
        unit = grl::Vec3f(0.0f, 2.0f, 0.0f);
        unit.normalize();
        Assert::AreEqual(0.0f, unit.x);
        Assert::AreEqual(1.0f, unit.y);
        Assert::AreEqual(0.0f, unit.z);
        unit = grl::Vec3f(0.0f, 0.0f, 12.0f);
        unit.normalize();
        Assert::AreEqual(0.0f, unit.x);
        Assert::AreEqual(0.0f, unit.y);
        Assert::AreEqual(1.0f, unit.z);

        vec1.normalize();
        grl::Vec3f normalizedVec2 = grl::Vec3f::normalize(vec2);
        Assert::AreEqual(1.0f, vec1.length(), floatTolerance);
        Assert::AreEqual(1.0f, normalizedVec2.length(), floatTolerance);

        Logger::WriteMessage("----normalize Done");
    }

    TEST_METHOD(distance)
    {
        Logger::WriteMessage("----In length");

        float dist1 = grl::Vec3f::distance(vec1, vec2);
        float dist2 = grl::Vec3f::distance(vec2, vec1);
        Assert::AreEqual(dist1, dist2);
        Assert::AreEqual(8.246f, dist1, floatTolerance);

        Logger::WriteMessage("----length Done");
    }

    TEST_METHOD(operatorSum)
    {
        Logger::WriteMessage("----In operatorSum");

        grl::Vec3f sum = vec1 + vec2;
        Assert::AreEqual(6.0f, sum.x);
        Assert::AreEqual(0.0f, sum.y);
        Assert::AreEqual(0.0f, sum.z);

        vec1 += vec2;
        Assert::AreEqual(6.0f, vec1.x);
        Assert::AreEqual(0.0f, vec1.y);
        Assert::AreEqual(0.0f, vec1.z);

        Logger::WriteMessage("----operatorSum Done");
    }

    TEST_METHOD(operatorSubtract)
    {
        Logger::WriteMessage("----In operatorSubtract");

        grl::Vec3f sub1 = vec1 - vec2;
        Assert::AreEqual(-4.0f, sub1.x);
        Assert::AreEqual(6.0f, sub1.y);
        Assert::AreEqual(-4.0f, sub1.z);

        grl::Vec3f sub2 = vec2 - vec1;
        Assert::AreEqual(4.0f, sub2.x);
        Assert::AreEqual(-6.0f, sub2.y);
        Assert::AreEqual(4.0f, sub2.z);

        vec1 -= vec2;
        Assert::AreEqual(-4.0f, vec1.x);
        Assert::AreEqual(6.0f, vec1.y);
        Assert::AreEqual(-4.0f, vec1.z);

        Logger::WriteMessage("----operatorSubtract Done");
    }

    TEST_METHOD(operatorMultiply)
    {
        Logger::WriteMessage("----In operatorMultiply");

        float val = -2.5f;
        grl::Vec3f mul1 = vec1 * val;
        Assert::AreEqual(-2.5f, mul1.x);
        Assert::AreEqual(-7.5f, mul1.y);
        Assert::AreEqual(5.0f, mul1.z);
        grl::Vec3f mul2 = val * vec1;
        Assert::AreEqual(-2.5f, mul2.x);
        Assert::AreEqual(-7.5f, mul2.y);
        Assert::AreEqual(5.0f, mul2.z);

        vec2 *= val;
        Assert::AreEqual(-12.5f, vec2.x);
        Assert::AreEqual(7.5f, vec2.y);
        Assert::AreEqual(-5.0f, vec2.z);

        Logger::WriteMessage("----operatorMultiply Done");
    }

    TEST_METHOD(operatorDivide)
    {
        Logger::WriteMessage("----In operatorDivide");

        float val = 2.0f;
        grl::Vec3f div1 = vec1 / val;
        Assert::AreEqual(0.5f, div1.x, floatTolerance);
        Assert::AreEqual(1.5f, div1.y, floatTolerance);
        Assert::AreEqual(-1.0f, div1.z, floatTolerance);

        vec2 /= val;
        Assert::AreEqual(2.5f, vec2.x, floatTolerance);
        Assert::AreEqual(-1.5f, vec2.y, floatTolerance);
        Assert::AreEqual(1.0f, vec2.z, floatTolerance);

        Logger::WriteMessage("----operatorDivide Done");
    }
};

TEST_CLASS(PlaneTester)
{
private:
    static constexpr float floatTolerance = 0.001f;

    grl::Vec3f p0;
    grl::Plane plane;
    grl::Plane planeXY;
    grl::Plane planeXZ;
    grl::Plane planeYZ;
    grl::Plane planeXYInv;
public:
    PlaneTester()
    {
        Logger::WriteMessage("--In PlaneTester");
    }

    ~PlaneTester()
    {
        Logger::WriteMessage("--PlaneTester Done");
    }

    TEST_METHOD_INITIALIZE(preparePlanes)
    {
        grl::Vec3f normal = grl::Vec3f::normalize(grl::Vec3f(15.0f, 94.0f, 127.0f));
        p0 = grl::Vec3f(1.0f, -4.0f, 8.0f);

        plane.set(normal, p0);
        planeXY.set(grl::Vec3f(0.0f, 0.0f, 1.0f), p0);
        planeXZ.set(grl::Vec3f(0.0f, 1.0f, 0.0f), p0);
        planeYZ.set(grl::Vec3f(1.0f, 0.0f, 0.0f), p0);
        planeXYInv.set(grl::Vec3f(0.0f, 0.0f, -1.0f), p0);
    }

    TEST_METHOD(function)
    {
        Logger::WriteMessage("----In function");

        // Plane XY
        // On plane
        Assert::AreEqual(0.0f, planeXY(p0), floatTolerance);
        Assert::AreEqual(0.0f, planeXY(p0 + grl::Vec3f(5.0f, -3.0f, 0.0f)), floatTolerance);
        // Behind plane
        Assert::AreEqual(-5.0f, planeXY(p0 + grl::Vec3f(0.0f, 0.0f, -5.0f)), floatTolerance);
        Assert::IsTrue(planeXY(p0+ grl::Vec3f(-8.0f, 6.0f, -1.0f)) < 0.0f);
        // In front of plane
        Assert::AreEqual(3.0f, planeXY(p0 + grl::Vec3f(0.0f, 0.0f, 3.0f)), floatTolerance);
        Assert::IsTrue(planeXY(p0+ grl::Vec3f(5.0f, 4.0f, 9.0f)) > 0.0f);

        // Plane YZ
        // On plane
        Assert::AreEqual(0.0f, planeYZ(p0), floatTolerance);
        Assert::AreEqual(0.0f, planeYZ(p0 + grl::Vec3f(0.0f, 2.0f, 3.0f)), floatTolerance);
        // Behind plane
        Assert::AreEqual(-5.0f, planeYZ(p0 + grl::Vec3f(-5.0f, 0.0f, 0.0f)), floatTolerance);
        Assert::IsTrue(planeXY(p0+ grl::Vec3f(-8.0f, 6.0f, -1.0f)) < 0.0f);
        // In front of plane
        Assert::AreEqual(3.0f, planeYZ(p0 + grl::Vec3f(3.0f, 0.0f, 0.0f)), floatTolerance);
        Assert::IsTrue(planeYZ(p0+ grl::Vec3f(5.0f, 4.0f, 9.0f)) > 0.0f);

        // Plane XZ
        // On plane
        Assert::AreEqual(0.0f, planeXZ(p0), floatTolerance);
        Assert::AreEqual(0.0f, planeXZ(p0 + grl::Vec3f(-5.0f, 0.0f, 3.0f)), floatTolerance);
        // Behind plane
        Assert::AreEqual(-5.0f, planeXZ(p0 + grl::Vec3f(0.0f, -5.0f, 0.0f)), floatTolerance);
        Assert::IsTrue(planeXZ(p0+ grl::Vec3f(-3.0f, -6.0f, -2.0f)) < 0.0f);
        // In front of plane
        Assert::AreEqual(3.0f, planeXZ(p0 + grl::Vec3f(0.0f, 3.0f, 0.0f)), floatTolerance);
        Assert::IsTrue(planeXZ(p0+ grl::Vec3f(-3.0f, 6.0f, 8.0f)) > 0.0f);

        // Plane XY inverted
        // On plane
        Assert::AreEqual(0.0f, planeXYInv(p0), floatTolerance);
        Assert::AreEqual(0.0f, planeXYInv(p0 + grl::Vec3f(5.0f, -3.0f, 0.0f)), floatTolerance);
        // Behind plane
        Assert::AreEqual(-3.0f, planeXYInv(p0 + grl::Vec3f(0.0f, 0.0f, 3.0f)), floatTolerance);
        Assert::IsTrue(planeXYInv(p0+ grl::Vec3f(5.0f, 4.0f, 9.0f)) < 0.0f);
        // In front of plane
        Assert::AreEqual(5.0f, planeXYInv(p0 + grl::Vec3f(0.0f, 0.0f, -5.0f)), floatTolerance);
        Assert::IsTrue(planeXYInv(p0+ grl::Vec3f(-8.0f, 6.0f, -1.0f)) > 0.0f);

        // Plane 3D
        // On plane
        Assert::AreEqual(0.0f, plane(p0), floatTolerance);
        // Behind plane
        Assert::IsTrue(plane(p0+ grl::Vec3f(-5.0f, -4.0f, -10.0f)) < 0.0f);
        // In front of plane
        Assert::IsTrue(plane(p0+ grl::Vec3f(2.0f, 4.0f, 13.0f)) > 0.0f);

        Logger::WriteMessage("----function Done");
    }

    TEST_METHOD(isOnPlane)
    {
        Logger::WriteMessage("----In isOnPlane");

        // Plane XY
        Assert::IsTrue(planeXY.isOnPlane(p0));
        Assert::IsTrue(planeXY.isOnPlane(p0 + grl::Vec3f(5.0f, -3.0f, 0.0f)));
        Assert::IsFalse(planeXY.isOnPlane(p0 + grl::Vec3f(1.0f, -1.0f, 1.0f)));

        // Plane YZ
        Assert::IsTrue(planeYZ.isOnPlane(p0));
        Assert::IsTrue(planeYZ.isOnPlane(p0 + grl::Vec3f(0.0f, 2.0f, 3.0f)));
        Assert::IsFalse(planeYZ.isOnPlane(p0 + grl::Vec3f(1.0f, -1.0f, 1.0f)));

        // Plane XZ
        Assert::IsTrue(planeXZ.isOnPlane(p0));
        Assert::IsTrue(planeXZ.isOnPlane(p0 + grl::Vec3f(-5.0f, 0.0f, 3.0f)));
        Assert::IsFalse(planeXZ.isOnPlane(p0 + grl::Vec3f(1.0f, -1.0f, 1.0f)));

        // Plane XY inverted
        Assert::IsTrue(planeXYInv.isOnPlane(p0));
        Assert::IsTrue(planeXYInv.isOnPlane(p0 + grl::Vec3f(5.0f, -3.0f, 0.0f)));
        Assert::IsFalse(planeXYInv.isOnPlane(p0 + grl::Vec3f(1.0f, -1.0f, 1.0f)));

        // Plane 3D
        // On plane
        Assert::IsTrue(plane.isOnPlane(p0));
        Assert::IsFalse(plane.isOnPlane(p0 + grl::Vec3f(1.0f, -1.0f, 1.0f)));

        Logger::WriteMessage("----isOnPlane Done");
    }

    TEST_METHOD(fromPoints)
    {
        Logger::WriteMessage("----In fromPoints");

        grl::Vec3f p1(1.0f, 2.0f, 4.0f);
        grl::Vec3f p2(3.0f, 2.0f, 2.0f);
        grl::Vec3f p3(2.0f, 2.0f, 5.0f);
        grl::Plane plane = grl::Plane::fromPoints(p1, p2, p3);

        Assert::IsTrue(plane.isOnPlane(p1));
        Assert::IsTrue(plane.isOnPlane(p2));
        Assert::IsTrue(plane.isOnPlane(p3));
        Assert::AreEqual(0.0f, plane.getNormal().x, floatTolerance);
        Assert::AreEqual(1.0f, plane.getNormal().y, floatTolerance);
        Assert::AreEqual(0.0f, plane.getNormal().z, floatTolerance);

        Logger::WriteMessage("----fromPoints Done");
    }

    TEST_METHOD(fromVectors)
    {
        Logger::WriteMessage("----In fromVectors");

        grl::Vec3f p0(3.0f, 2.0f, 2.0f);
        grl::Vec3f v1(-2.0f, 0.0f, 2.0f);
        grl::Vec3f v2(-1.0f, 0.0f, 3.0f);
        grl::Plane plane = grl::Plane::fromVectors(p0, v1, v2);

        Assert::IsTrue(plane.isOnPlane(p0));
        Assert::AreEqual(0.0f, plane.getNormal().x, floatTolerance);
        Assert::AreEqual(1.0f, plane.getNormal().y, floatTolerance);
        Assert::AreEqual(0.0f, plane.getNormal().z, floatTolerance);

        Logger::WriteMessage("----fromVectors Done");
    }
};

TEST_CLASS(OrientedTransformationTester)
{
private:
    static constexpr float floatTolerance = 0.001f;
public:
    OrientedTransformationTester()
    {
        Logger::WriteMessage("--In OrientedTransformationTester");
    }

    ~OrientedTransformationTester()
    {
        Logger::WriteMessage("--OrientedTransformationTester done");
    }

    TEST_METHOD(all)
    {
        Logger::WriteMessage("----In all");

        grl::OrientedTransformation ot(5.0f, grl::Vec3f(1.0f, 5.0f, 2.0f), false);
        Assert::AreEqual(1.0f, ot.getUnitVector().length(), floatTolerance);
        Assert::AreEqual(5.0f, ot.getTransformationVector().length(), floatTolerance);

        ot = grl::OrientedTransformation(grl::Vec3f(5.0f, 0.0f, 0.0f));
        Assert::AreEqual(1.0f, ot.getUnitVector().length(), floatTolerance);
        Assert::AreEqual(1.0f, ot.getUnitVector().x, floatTolerance);
        Assert::AreEqual(5.0f, ot.getTransformationVector().length(), floatTolerance);
        Assert::AreEqual(5.0f, ot.getTransformationVector().x, floatTolerance);
        Assert::AreEqual(5.0f, ot.getDistance(), floatTolerance);

        Logger::WriteMessage("----all Done");
    }
};

}
