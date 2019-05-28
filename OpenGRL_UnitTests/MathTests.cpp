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
        float values[valuesSize] = { 5.2f, -1.53f};

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
        int values[valuesSize] = { 12, -12, -1, 1, 0};
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
        float values[valuesSize] = { 6.4f, -2.5, -0.001f, 0.001f, 0.0f};
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
}
