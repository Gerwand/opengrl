#pragma once

#include "DecisionTree.h"

#include <thread>
#include <list>

namespace grl {

struct ForestTrainGPUContext
{
    cl::Device device;
    cl::Context context;
    // Must not be set by the main app - the tree will set it by itself
    cl::Program program;
    int maxImages;
};

struct ForestTrainContext
{
    ForestTrainGPUContext *gpuContext;
    size_t nthreads;
    size_t pixelsPerImage;
    int nodeTrainLimit;
    int maxDepth;
    std::vector<cv::Mat> classImages;
    std::vector<cv::Mat> depthImages;
};

using ClassesWeights = std::array<cv::Mat, grlHandIndexNum>;
using ClassesPoints = std::array<std::list<Vec2i>, grlHandIndexNum>;
using ClassesValues = std::array<std::list<float>, grlHandIndexNum>;

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
};

inline 
RandomDecisionForest::RandomDecisionForest(size_t ntrees)
{
    _trees.resize(ntrees);
}

}
