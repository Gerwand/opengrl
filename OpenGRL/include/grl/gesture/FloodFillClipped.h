#pragma once

#include <grl/gesture/GestureExtractor.h>

#include <array>

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>

namespace grl {

/**
 * Expand normal Voxel implementation which are containing additional
 * information required by the FloodFill algorithm.
 */
constexpr size_t MaxFFVoxelNeighbours = 8;
struct FFVoxel : public Voxel
{
    // For faster access to the data, all neighbours are stored inside the
    // voxel.
    std::array<FFVoxel *, MaxFFVoxelNeighbours> neighbours;
    // Number of neighbours that are stored.
    int neighboursNumber;

    /**
     * Basic constructor, doing nothing.
     */
    FFVoxel() {}

    /**
     * Constructor initializing all fields and setting defaults.
     *
     * @param x x coordinate of the voxel on the image
     * @param y y coordinate of the voxel on the image
     * @param z depth of the voxel
     */
    FFVoxel(int x, int y, int z);
};

/**
 * Representation of the voxels in the 2D spaces, can be used for converting
 * image to the voxel 2D array.
 */
class VoxelArray2D
{
public:
    /**
     * Destructor, which is releasing array of voxels if it was allocated but
     * do not released.
     */
    ~VoxelArray2D();

    /**
     * Initialize voxel array with given with and height. The FFVoxels are
     * having set default depth to 0 and are not being modified otherwise.
     * All adjacent voxels for each voxel are being set as a neighbours.
     *
     * @param width widht of the voxel array.
     * @param height height of the voxel array.
     *
     * @returns true if the array was successfully created
     */
    bool init(int width, int height);

    /**
     * Create voxel array using image from data. This function is doing further
     * optimization and is adding adjacent voxels as a neighbours to the voxel
     * only if the distance between the depth of two voxels is smaller or equal
     * to depthTolerance.
     *
     * @param depthImage depth image, from which the voxel array will be created.
     * It should be uint16_t type.
     * @param depthTolerance maximum depth difference between two adjacent
     * voxels, to consider them as a neighbours.
     * @returns true if the array was successfully created
     */
    bool fromImage(const cv::Mat &depthImage, int depthTolerance = INT_MAX);

    /**
     * Release all allocated resources inside the VoxelArray2D.
     */
    void destroy();

    /**
     * Get reference to the voxel at the given coordinates.
     *
     * @param x x coordinate of the voxel.
     * @param y y coordinate of the voxel.
     * @returns reference to the voxel in the 2D array.
     */
    FFVoxel & getVoxel(int x, int y);

    /**
     * Get reference to the voxel at the given coordinates.
     *
     * @param x x coordinate of the voxel.
     * @param y y coordinate of the voxel.
     * @returns reference to the voxel in the 2D array.
     */
    const FFVoxel & getVoxel(int x, int y) const;

    /**
     * Get reference to the voxel at the given coordinates.
     *
     * @param x x coordinate of the voxel.
     * @param y y coordinate of the voxel.
     * @returns reference to the voxel in the 2D array.
     */
    FFVoxel & operator()(int x, int y);

    /**
     * Get value of the voxel at the given coordinates.
     *
     * @param x x coordinate of the voxel.
     * @param y y coordinate of the voxel.
     * @returns value of the voxel
     */
    int getVoxelValue(int x, int y) const;

    /**
     * Set value of the voxel at the given coordinates. It won't cause
     * recalculation of the neighbours.
     *
     * @param x x coordinate of the voxel.
     * @param y y coordinate of the voxel.
     */
    void setVoxelValue(int x, int y, int value);

    /**
     * Get size of the voxel array.
     *
     * @param[out] width width of the voxel array.
     * @param[out] height height of the voxel array.
     */
    void getSize(int &width, int &height) const;

private:
    int _width, _height;
    FFVoxel *_voxels = nullptr;
};

/**
 * Class implementing FloodFill algorithm but it is extracting the object which
 * can be clipped by the plane. Only the points in fron of the plane will be
 * extracted and it is possible to create 3D plane so the value of each pixel
 * will be also taken into consideration.
 */
class FloodFillClipped
{
public:
    /**
     * Initialize the object using the input source data and the tolerance to
     * determine, if the pixel should be part of the object or not.
     *
     * @param tolerance maximum distance between two values of two pixels, to
     * add the adjacent pixel to the object.
     * @param source image from which the objects should be extracted.
     */
    void init(int tolerance, const cv::Mat &source);

    /**
     * Extract signle object starting from the given point using the floodfill
     * algorithm. All points behind the plane won't be treated as a part of the
     * object.
     *
     * @param[in] startingPoint the point in the image from which the extraction
     * should start.
     * @param[in] plane plane which is additionally clipping the object.
     * @param[out] object structure, to which the object is being saved.
     * @returns true if the object was extraced
     */
    bool extractObject(Vec2i startingPoint, Plane plane, DepthObject &object);
private:
    int _tolerance;
    VoxelArray2D _voxelImage;
    std::vector<bool> _usedMap;
};

}
