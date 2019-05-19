#pragma once

#ifdef USE_GPU
#include <CL/cl.hpp>
#endif

#include <grl/rdf/SegmentedDepthUtils.h>

#include <cassert>
#include <vector>
#include <memory>
#include <limits>
#include <random>
#include <fstream>
#include <iostream>


namespace grl {

#ifdef USE_GPU
struct TreeTrainGPUContext
{
    // Delivered to the tree
    cl::Device device;
    cl::Context context;
    cl::Program program;
    cl::CommandQueue queue;
    cl::Buffer bufferAllPix;
    cl::Buffer bufferSplit;
    cl::Buffer bufferPix;
    cl::Buffer bufferPixCount;
    // programs
    cl::Kernel getFeatureTrain;
    cl::Kernel getProbabilities;
    int maxImages;
};

#pragma pack(push, 1)
struct Decision
{
    // Offsets (feature)
    cl_int2 u;
    cl_int2 v;
    // Threshold
    cl_float t;
};
#pragma pack(pop)
#else // !USE_GPU
typedef void * TreeTrainGPUContext;

struct Decision
{
    // Offsets (feature)
    Vec2i u;
    Vec2i v;
    // Threshold
    float t;
};
#endif

constexpr uint8_t grlNodeGoLeft = 0;
constexpr uint8_t grlNodeGoRight = 1;

class Node
{
public:
    Node(Node *parent = nullptr);
    Node(std::vector<Pixel> *pixels, const std::vector<float> &probabilities, Node *parent = nullptr);
    Node(const Decision &decision, Node *parent = nullptr);
    Node(const std::vector<float> &probabilities, Node *parent = nullptr);

    void setPixels(std::vector<Pixel> *pixels) { _pixels = pixels; }
    std::vector<Pixel> * getPixels() { return _pixels; }

    Node * getLeft() { return _left.get(); }
    Node * getRight() { return _right.get(); }
    const Node * getLeft() const { return _left.get(); }
    const Node * getRight() const { return _right.get(); }
    void setLeft(std::unique_ptr<Node> left) { _left = std::move(left); }
    void setRight(std::unique_ptr<Node> right) { _right = std::move(right); }

    Node * getParent() { return _parent; }

    void setDecision(const Decision &decision);

    void setProbabilities(const std::vector<float> &probabilities);

    const std::vector<float> & getProbabilities() const { return _probabilities; }

    bool isLeaf() const { return _isLeaf; }
    void setLeaf(bool leaf);

    const Decision & getDecision() { return _decision; }

    uint8_t evaluateFeature(const cv::Mat &depthImage, const Pixel &p) const;

    void saveToFile(std::ofstream &file);

    const Node * getLeafForPixel(const cv::Mat &depthImage, const Pixel &p);

private:
    bool _isLeaf = false;
    Node *_parent;
    std::vector<Pixel> *_pixels = nullptr;
    std::unique_ptr<Node> _left;
    std::unique_ptr<Node> _right;
    // Used for training
    
    Decision _decision;
    std::vector<float> _probabilities;
};

inline
Node::Node(Node *parent) 
    : _parent(parent) 
{
}

inline
Node::Node(std::vector<Pixel> *pixels, const std::vector<float> &probabilities, Node *parent)
    : _parent(parent)
    , _pixels(pixels)
{ 
    setProbabilities(probabilities);
}

inline
Node::Node(const Decision &decision, Node *parent)
    : _parent(parent)
{ 
    setDecision(decision);
}

inline
Node::Node(const std::vector<float> &probabilities, Node *parent)
    : _parent(parent)
{ 
    setProbabilities(probabilities);
}

inline void
Node::setDecision(const Decision &decision)
{
    _isLeaf = false;
    _decision = decision;
}

inline void
Node::setProbabilities(const std::vector<float> &probabilities)
{
    _isLeaf = true;
    _probabilities = probabilities;
}

class DecisionTree
{
public:
    void train(std::vector<Pixel> *pixels, const std::vector<cv::Mat> &depthImages,
               int nodeTrainLimit, int maxDepth, std::mt19937 &gen, TreeTrainGPUContext *gpuContext);

    void setRoot(std::unique_ptr<Node> root) { _root = std::move(root); }
    Node * getRoot() { return _root.get(); }

    void saveToFile(std::ofstream &file);
    void readFromFile(std::ifstream & file);

    const std::vector<float> & classifyPixel(const cv::Mat &depthImage, const Pixel &p);

private:
    struct NodeTrainingData {
        std::vector<Pixel> *allPixels;
        std::vector<float> allProbabilities;
        std::vector<uint16_t> imageIDs;
        std::vector<int> imagesPixelCount;

        std::vector<Pixel> *leftPixels;
        std::vector<float> leftProbabilities;

        std::vector<Pixel> *rightPixels;
        std::vector<float> rightProbabilities;
    };

    std::unique_ptr<Node> _root;
    Vec2i _offsetsRange;
    float _maxThresh;
    int _maxDepth;

    void getProbabilities(const std::vector<Pixel> *pixels, std::vector<float> &probabilities);
#ifdef USE_GPU

    void getProbabilities(const std::vector<Pixel> *pixels, std::vector<float> &probabilities,
                          TreeTrainGPUContext *gpuContext);
#endif

    bool isSingleClass(const std::vector<float> &probabilities);

    float getShannonEntropy(const std::vector<float> &probabilities);

    float evaluateNode(Node *node, NodeTrainingData &data, const std::vector<cv::Mat> &depthImages,
                       TreeTrainGPUContext *gpuContext);
    float evaluateNode(Node *node, NodeTrainingData &data, const std::vector<cv::Mat> &depthImages);
};

inline bool
DecisionTree::isSingleClass(const std::vector<float> &probabilities)
{
    int notZero = 0;
    for (auto it = probabilities.cbegin(); it != probabilities.cend(); ++it)
        if (*it != 0.0f)
            ++notZero;

    return notZero == 1;
}

inline float 
DecisionTree::getShannonEntropy(const std::vector<float> &probabilities)
{
    float entropy = 0.0f;
    for (auto it = probabilities.cbegin(); it != probabilities.cend(); ++it) {
        float val = *it;
        if (val != 0.0f)
            entropy += val*log2(val);
    }

    return -entropy;
}

inline void
DecisionTree::saveToFile(std::ofstream & file)
{
    assert(file.is_open());

    if (_root.get() != nullptr)
        _root->saveToFile(file);
}

}