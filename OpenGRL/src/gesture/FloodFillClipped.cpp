#include <grl/gesture/FloodFillClipped.h>
#include <queue>

namespace grl {

//////////////////////////////////////////////////
// FFVoxel
//////////////////////////////////////////////////

FFVoxel::FFVoxel(int x, int y, int z)
{
    coords = Vec3i(x, y, z);
}

//////////////////////////////////////////////////
// VoxelArray2D
//////////////////////////////////////////////////

VoxelArray2D::~VoxelArray2D()
{
    destroy();
}

FFVoxel & VoxelArray2D::operator()(int x, int y)
{
    return getVoxel(x, y);
}

FFVoxel & VoxelArray2D::getVoxel(int x, int y)
{
    return _voxels[x + y * _width];
}

const FFVoxel & VoxelArray2D::getVoxel(int x, int y) const
{
    return _voxels[x + y * _width];
}

int VoxelArray2D::getVoxelValue(int x, int y) const
{
    return getVoxel(x, y).coords.z;
}

void VoxelArray2D::setVoxelValue(int x, int y, int value)
{
    getVoxel(x, y).coords.z = value;
}

void VoxelArray2D::getSize(int &width, int &height) const
{
    width = _width;
    height = _height;
}

void VoxelArray2D::destroy()
{
    delete _voxels;
    _voxels = nullptr;
}

bool VoxelArray2D::init(int width, int height)
{
    // To prevent any leaks, call destroy just in case
    destroy();

    _voxels = new FFVoxel[width*height];
    if (_voxels == nullptr)
        return false;

    _width = width;
    _height = height;

    // Initialize every voxel in the structure
    FFVoxel *voxel = _voxels;
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w, ++voxel) {
            *voxel = FFVoxel(w, h, 0);

            // Fill neighbours array
            voxel->neighboursNumber = 0;
            for (int y = clampMin(h - 1, 0); y <= clampMax(h + 1, height - 1); ++y) {
                for (int x = clampMin(w - 1, 0); x <= clampMax(w + 1, width - 1); ++x) {
                    // Make sure it is not the same voxel
                    if (y != h || x != w)
                        voxel->neighbours[voxel->neighboursNumber++] = &getVoxel(x, y);
                }
            }
        }
    }

    return true;
}

bool VoxelArray2D::fromImage(const cv::Mat &image, int tolerance)
{
    // Make sure that this is depth image.
    assert(image.type() == CV_16UC1);

    // To prevent any leaks, call destroy just in case
    destroy();

    _voxels = new FFVoxel[image.rows * image.cols];
    if (_voxels == nullptr)
        return false;

    _width = image.cols;
    _height = image.rows;

    // Initialize every voxel in the structure
    FFVoxel *voxel = _voxels;
    // Access the image data as a raw data
    const uint16_t *dataStart = reinterpret_cast<const uint16_t *>(image.data);
    const uint16_t *pixel = reinterpret_cast<const uint16_t *>(image.data);
    for (int h = 0; h < image.rows; ++h) {
        for (int w = 0; w < image.cols; ++w, ++voxel, ++pixel) {
            *voxel = FFVoxel(w, h, *pixel);

            // Fill neighbours array
            voxel->neighboursNumber = 0;
            for (int y = clampMin(h - 1, 0); y <= clampMax(h + 1, image.rows - 1); ++y) {
                for (int x = clampMin(w - 1, 0); x <= clampMax(w + 1, image.cols - 1); ++x) {
                    // Make sure it is not the same voxel
                    if (y != h || x != w) {
                        // Get neigbour only if the diff of the value is lower
                        // than the tolerance - that will reduce number of
                        // neighbours to be checked
                        const uint16_t *neighbour = dataStart + x + y*image.cols;
                        int distance = static_cast<int>(*neighbour) - static_cast<int>(*pixel);
                        if (absBetween(distance, tolerance))
                            voxel->neighbours[voxel->neighboursNumber++] = &getVoxel(x, y);
                    }
                }
            }
        }
    }

    return true;
}

//////////////////////////////////////////////////
// FloodFillClipped
//////////////////////////////////////////////////

void FloodFillClipped::init(int tolerance, const cv::Mat &source)
{
    _tolerance = tolerance;
    _voxelImage.fromImage(source, tolerance);
    _usedMap.resize(source.rows*source.cols);
}

bool FloodFillClipped::extractObject(Vec2i startingPoint, Plane plane, DepthObject &object)
{
    object.reset();

    int width, height;
    _voxelImage.getSize(width, height);

    std::fill(_usedMap.begin(), _usedMap.end(), false);
    //std::vector<size_t> toFalse;

    // Create queue for all voxels that must be analyzed
    std::queue<FFVoxel *> enqueuedVoxels;
    FFVoxel *firstVoxel = &_voxelImage.getVoxel(startingPoint.x, startingPoint.y);

    // If the first voxel is behind the plane, skip extraction of the object
    Vec3f voxelCoords = Vec3f(static_cast<float>(firstVoxel->coords.x),
                                  static_cast<float>(firstVoxel->coords.y),
                                  static_cast<float>(firstVoxel->coords.z));
    if (plane(voxelCoords) < 0.0f)
        return false;

    // Put first voxel to the queue
    enqueuedVoxels.push(firstVoxel);

    // Check all voxels until final solution is found
    while (!enqueuedVoxels.empty()) {
        // Dequeue the first voxel in the queue
        FFVoxel *currentVoxel = enqueuedVoxels.front();
        enqueuedVoxels.pop();

        size_t voxelIndex = currentVoxel->coords.x + currentVoxel->coords.y*width;

        // As the analyzis begun, the voxel can be set that it was analyzed
        // and added as a part of the object
        _usedMap[voxelIndex] = true;
        //toFalse.push_back(voxelIndex);
        object.putVoxel(currentVoxel);

        // Analyze all neighbours. As we know that the VoxelArray was
        // initialized using image and a tolerance, we can be sure that it will
        // contain only those neighbours that are part of the object as a whole
        for (int i = 0; i < currentVoxel->neighboursNumber; ++i) {
            FFVoxel *neighbour = currentVoxel->neighbours[i];
            size_t neighbourIndex = neighbour->coords.x + neighbour->coords.y*width;
            Vec3f neighbourCoords(static_cast<float>(neighbour->coords.x),
                                  static_cast<float>(neighbour->coords.y),
                                  static_cast<float>(neighbour->coords.z));
            // Push to queue only if the neigbour wasn't already analyzed and if
            // the object is in front of or on the plane.
            if (!_usedMap[neighbourIndex] && plane(neighbourCoords) >= 0.0f) {
                _usedMap[neighbourIndex] = true;
                //toFalse.push_back(neighbourIndex);
                enqueuedVoxels.push(neighbour);
            }
        }
    }

    //for (auto it = toFalse.begin(); it != toFalse.end(); ++it)
    {
      //  _usedMap[*it] = false;
    }

    return true;
}

}
