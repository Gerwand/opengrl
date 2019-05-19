#pragma once

#include <grl/gesture/GestureExtractor.h>

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>

namespace grl {

#define UNKNOWN INT8_MIN
#define BEHIND_PLANE -1
#define ON_PLANE 0
#define IN_FRONT_OF_PLANE 1


struct FFVoxel : public Voxel
{
	bool analyzed;
	int8_t planePosition = UNKNOWN;
	FFVoxel *neighbours[8];
	int neighboursNumber;

	void set(int x, int y, int z)
	{
		coords = Vec3i(x, y, z);
		analyzed = false;
		planePosition = UNKNOWN;
	}
};

struct VoxelArray2D
{
public:
	~VoxelArray2D();

	bool init(int width, int height);
	bool fromImage(const cv::Mat &image, int tolerance);
	void destroy();

	FFVoxel & getVoxel(int x, int y);
	int getVoxelValue(int x, int y);
	void setVoxelValue(int x, int y, int value);
	void getSize(int &width, int &height);
	FFVoxel * getData();

	FFVoxel & operator()(int x, int y);

	void toImage(cv::Mat &destination);
private:
	int _width, _height;
	FFVoxel *_voxels = nullptr;
};

inline
VoxelArray2D::~VoxelArray2D()
{
	destroy();
}

inline FFVoxel &
VoxelArray2D::operator()(int x, int y)
{
	return getVoxel(x, y);
}

inline FFVoxel &
VoxelArray2D::getVoxel(int x, int y)
{
	return _voxels[x + y * _width];
}

inline int
VoxelArray2D::getVoxelValue(int x, int y)
{
	return getVoxel(x, y).coords.z;
}

inline void
VoxelArray2D::setVoxelValue(int x, int y, int value)
{
	getVoxel(x, y).coords.z = value;
}

inline void
VoxelArray2D::getSize(int &width, int &height)
{
	width = _width;
	height = _height;
}

inline void
VoxelArray2D::destroy()
{
	delete _voxels;
	_voxels = nullptr;
}

inline FFVoxel *
VoxelArray2D::getData()
{
	return _voxels;
}

class FloodFillClipped
{
public:
	void setup(int tolerance, const cv::Mat &source);

	bool extractObject(Vec2i startingPoint, Object &object, Plane plane);
	bool isAnalyzed(Vec2i point);

private:
	int _tolerance;
	VoxelArray2D _voxelImage;

};

inline bool
FloodFillClipped::isAnalyzed(Vec2i point)
{
	return _voxelImage.getVoxel(point.x, point.y).analyzed;
}

inline void
FloodFillClipped::setup(int tolerance, const cv::Mat &source)
{
	_tolerance = tolerance;
	_voxelImage.fromImage(source, tolerance);
}

}