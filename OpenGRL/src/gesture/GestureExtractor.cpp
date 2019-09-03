#include <grl/gesture/GestureExtractor.h>

namespace grl {

//////////////////////////////////////////////////
// DepthObject
//////////////////////////////////////////////////

DepthObject::DepthObject()
{
    reset();
}

void DepthObject::putVoxel(const Voxel *voxel)
{
    _voxels.push_back(voxel);

    // The bounding box and depth ranges could be changed, so it must be
    // checked.
    // This can be reworked, so the maintainer of this object will make sure
    // that the object is being recalcualted after all voxels are being added
    // to the object.
    recalculate3DBoundingBox(voxel);

    _objectChanged = true;
}

void DepthObject::recalculate3DBoundingBox(const Voxel *voxel)
{
    // Recalculate bounding box (width and height)
    // Width
    if (voxel->coords.x < _boundingBox.x) {
        _boundingBox.x = voxel->coords.x;
        _boundingBox.width = _maxX - _boundingBox.x + 1;
    }
    // No else, as when the first voxel is being put the coordinates are both
    // smaller than bounding box (INT_MAX) and larger than max x (INT_MIN).
    // If the object would be containing only from the single voxel, it's state
    // and width would be invalid.
    if (voxel->coords.x > _maxX) {
        _maxX = voxel->coords.x;
        // We must keep
        _boundingBox.width = _maxX - _boundingBox.x + 1;
    }

    // Height
    if (voxel->coords.y > _maxY) {
        _maxY = voxel->coords.y;
        _boundingBox.height = _maxY - _boundingBox.y + 1;
    }
    // Same comment for else as for the width.
    if (voxel->coords.y < _boundingBox.y) {
        _boundingBox.y = voxel->coords.y;
        _boundingBox.height = _maxY - _boundingBox.y + 1;
    }

    // Recalculate min and max depth (ranges)
    if (voxel->coords.z < _minDepth)
        _minDepth = voxel->coords.z;
    if (voxel->coords.z > _maxDepth)
        _maxDepth = voxel->coords.z;
}

size_t DepthObject::getSize() const
{
    return _voxels.size();
}

void DepthObject::reset()
{
    _voxels.clear();
    _depthImage = cv::Mat();
    _objectChanged = false;
    _minDepth = INT_MAX;
    _maxDepth = INT_MIN;
    _boundingBox = cv::Rect(INT_MAX, INT_MAX, 0, 0);
    _maxX = INT_MIN;
    _maxY = INT_MIN;
    _accuracy = 0;
}

void
DepthObject::generateImage() const
{
    _depthImage = cv::Mat::zeros(_boundingBox.height, _boundingBox.width, CV_16UC1);
    uint16_t *imgData = reinterpret_cast<uint16_t *>(_depthImage.data);

    auto itVoxel = _voxels.begin();
    for (; itVoxel != _voxels.end(); ++itVoxel) {
        const Voxel *voxel = *itVoxel;
        // The data can be accessed as an array, but the index must be calcualted
        int pixelIndex = (voxel->coords.x - _boundingBox.x) +
                         (voxel->coords.y - _boundingBox.y) * _boundingBox.width;
        imgData[pixelIndex] = static_cast<uint16_t>(voxel->coords.z);
    }
}

const std::vector<const Voxel *> & DepthObject::getVoxels() const
{
    return _voxels;
}

uint8_t DepthObject::getAccuracy() const
{
    return _accuracy;
}

void DepthObject::setAccuracy(uint8_t accuracy)
{
    _accuracy = accuracy;
}

int DepthObject::getMinDepthValue() const
{
    return _minDepth;
}

int DepthObject::getMaxDepthValue() const
{
    return _maxDepth;
}

void DepthObject::getDepthValuesRange(int &minDepth, int &maxDepth) const
{
    minDepth = _minDepth;
    maxDepth = _maxDepth;
}

const cv::Rect & DepthObject::getBoundingBox() const
{
    return _boundingBox;
}

const cv::Mat & DepthObject::getDepthImageOfObject() const
{
    if (_objectChanged)
        generateImage();

    _objectChanged = false;
    return _depthImage;
}

//////////////////////////////////////////////////
// GestureExtractor
//////////////////////////////////////////////////

bool GestureExtractor::init(const ExtractorConfig &config)
{
    return true;
}

void GestureExtractor::extractHands(const cv::Mat &depthImage,
                                    const Skeleton &skeleton,
                                    DepthObject &leftHand,
                                    DepthObject &rightHand)
{
    // Reset both objects just in case.
    leftHand.reset();
    rightHand.reset();

    bool leftValid = isHandValid(Side::Left, depthImage, skeleton);
    bool rightValid = isHandValid(Side::Right, depthImage, skeleton);

    if (leftValid || rightValid)
        prepareExtraction(depthImage, skeleton);

    if (rightValid)
        extractHand(Side::Right, depthImage, skeleton, rightHand);

    if (leftValid)
        extractHand(Side::Left, depthImage, skeleton, leftHand);
}

}
