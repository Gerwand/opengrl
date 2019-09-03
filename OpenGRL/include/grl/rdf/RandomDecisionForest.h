#pragma once

#include "DecisionTree.h"

#include <thread>
#include <list>
#include <map>

namespace grl {

#ifdef USE_GPU
struct ForestTrainGPUContext
{
    cl::Device device;
    cl::Context context;
    // Must not be set by the main app - the tree will set it by itself
    cl::Program program;
    int maxImages;
};
#endif

struct ForestTrainContext
{
#ifdef USE_GPU
    ForestTrainGPUContext *gpuContext;
#endif
    size_t nthreads;
    size_t pixelsPerImage;
    int nodeTrainLimit;
    int maxDepth;
    std::vector<cv::Mat> classImages;
    std::vector<cv::Mat> depthImages;
};

constexpr int grlBestPointsNum = 5;

using ClassesWeights = std::array<cv::Mat, grlHandIndexNum>;
using ClassesPoints = std::array<std::multimap<float, Vec2i>, grlHandIndexNum>;

class RandomDecisionForest
{
public:
    RandomDecisionForest(size_t ntrees = 0);

    DecisionTree & getTree(size_t i) { return _trees[i]; }
    const DecisionTree & getTree(size_t i) const { return _trees[i]; }

    DecisionTree & operator[](size_t i) { return _trees[i]; }
    const DecisionTree & operator[](size_t i) const { return _trees[i]; }

    void train(const ForestTrainContext &context);

    void saveToFile(const std::string &fileName);
    bool loadFromFile(const std::string &fileName);

    void classifyImage(const cv::Mat &depthImage, cv::Mat &classImage, ClassesWeights &weights,
        ClassesPoints &bestPoints);

private:
    std::vector<DecisionTree> _trees;
    std::vector<std::thread> _threads;

    static void trainTree(DecisionTree *tree, const ForestTrainContext *context);

    std::pair<float, int8_t> getClassForPixel(const cv::Mat &depthImage,
                                              const Pixel &pixel,
                                              std::vector<float> &probabilitiesSum);
};

inline
RandomDecisionForest::RandomDecisionForest(size_t ntrees)
{
    _trees.resize(ntrees);
}

}
