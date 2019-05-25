#include "stdafx.h"
#include "CppUnitTest.h"

#include <grl/utils/MathUtils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

constexpr size_t msgBufSize = 256;

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
            { {-4.91f,-4.9f}, {-4.9f,-4.89f}, {-4.89f,-4.91f}, {-8.0f, 5.0f}},
            { {10.321f,10.322f}, {10.320f,-10.321f}, {10.320f,10.322f}, {-9.0f, 20.0f}},
            { {-0.01f,0.0f}, {0.0f,0.01f}, {-0.01f,0.01f}, {-5.0f, 5.0f} }
        };
        MinMax<float> falseRanges[valuesSize][examplesSize] = {
            { {-4.92f,-4.91f}, {-4.90f,-4.89f}, {-5.0f,-11.0f}, {-2.0f,10.0f} },
            { {10.322f,10.323f}, {10.319f,10.320f}, {11.0f,15.0f}, {-15.0f,6.0f} },
            { {-0.02f,-0.01f}, {0.01f,0.02f}, {11.0f,15.0f}, {-15.0f,6.0f} },
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

};


}