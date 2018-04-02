#include "FloodFillClipped.h"

#include <queue>

namespace grl {

bool 
VoxelArray2D::init(int width, int height)
{
	destroy();

	_voxels = new FFVoxel[width*height];
	if (_voxels == nullptr)
		return false;

	_width = width;
	_height = height;
	FFVoxel *voxel = _voxels;
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			voxel->coords = Vec3i(w, h, 0);
			voxel->analyzed = false;
			voxel->planePosition = UNKNOWN;

			// Fill neighbours
			voxel->neighboursNumber = 0;
			for (int y = clampMin(h - 1, 0); y < clampMax(h + 1, height - 1); ++y) {
				for (int x = clampMin(w - 1, 0); w < clampMax(w + 1, width - 1); ++x) {
					if (y == h && x == w)
						continue;
					voxel->neighbours[voxel->neighboursNumber] = &getVoxel(x, y);
					++voxel->neighboursNumber;
				}
			}

			++voxel;
		}
	}	

	return true;
}

bool
VoxelArray2D::fromImage(const cv::Mat &image, int tolerance)
{
	assert(image.type() == CV_16UC1);

	destroy();

	_voxels = new FFVoxel[image.rows * image.cols];
	if (_voxels == nullptr)
		return false;

	_width = image.cols;
	_height = image.rows;
	FFVoxel *voxel = _voxels;
	const uint16_t *dataStart = reinterpret_cast<const uint16_t *>(image.data);
	const uint16_t *pixel = reinterpret_cast<const uint16_t *>(image.data);
	for (int h = 0; h < image.rows; ++h) {
		for (int w = 0; w < image.cols; ++w) {
			voxel->coords = Vec3i(w, h, *pixel);
			voxel->analyzed = false;
			voxel->planePosition = UNKNOWN;

			// Fill neighbours
			voxel->neighboursNumber = 0;
			for (int y = clampMin(h - 1, 0); y <= clampMax(h + 1, image.rows - 1); ++y) {
				for (int x = clampMin(w - 1, 0); x <= clampMax(w + 1, image.cols - 1); ++x) {
					if (y == h && x == w)
						continue;

					// Get neigbour only if the diff of the value is lower 
					// than the tolerance (for optimization)
					const uint16_t *neighbour = dataStart + x + y*image.cols;
					if (!absBetween(static_cast<int>(*neighbour) - static_cast<int>(*pixel), tolerance))
						continue;

					voxel->neighbours[voxel->neighboursNumber] = &getVoxel(x, y);
					++voxel->neighboursNumber;
				}
			}

			++voxel;
			++pixel;
		}
	}

	return true;
}

void 
VoxelArray2D::toImage(cv::Mat &destination)
{
	destination = cv::Mat(_height, _width, CV_16UC1);
	uint16_t *imgData = reinterpret_cast<uint16_t *>(destination.data);
	Voxel *voxel = _voxels;

	for (int i = 0; i < _height*_width; ++i, ++voxel, ++imgData)
		*imgData = static_cast<uint16_t>(voxel->coords.z);
}

bool 
FloodFillClipped::extractObject(Vec2i startingPoint, Object &object, Plane plane)
{
	/*
	Vec3i borderPoint(plane.p0.x, plane.p0.y, plane.p0.z);
	FFVoxel *firstBorderVoxel = &_voxelImage.getVoxel(borderPoint.x, borderPoint.y);

	if (firstBorderVoxel->analyzed)
		return false;

	// Set border from the clipping plane
	std::queue<FFVoxel *> enqueuedBorder;
	enqueuedBorder.push(firstBorderVoxel);
	while (!enqueuedBorder.empty()) {
		FFVoxel *currentVoxel = enqueuedBorder.front();
		enqueuedBorder.pop();

		if (currentVoxel->planePosition != UNKNOWN)
			continue;

		Vec3f voxelVector(currentVoxel->coords.x, currentVoxel->coords.y, currentVoxel->coords.z);

		// Check if voxel is on the plane
		float fPlane = plane(voxelVector);
		bool isOnPlane = isBetween(fPlane, 1.0f, -epsilon);

		//TODO: Check if the value is not too big...
		if (isOnPlane) {
			currentVoxel->planePosition = ON_PLANE;
		} else {
			// Not border, so check exact relation
			currentVoxel->planePosition = fPlane < 0.0f ? BEHIND_PLANE : IN_FRONT_OF_PLANE;
		}

		// If on plane or near the negative end of the plane
		if (isBetween(fPlane, 1.0f, -50.0f))
			// On plane, so add all neighbours for analyze
			for (int i = 0; i < currentVoxel->neighboursNumber; ++i) {
				FFVoxel *neighbour = currentVoxel->neighbours[i];
				if (neighbour->planePosition == UNKNOWN)
					enqueuedBorder.push(neighbour);
			}

	}
	*/
	FFVoxel *firstVoxel = &_voxelImage.getVoxel(startingPoint.x, startingPoint.y);
	std::queue<FFVoxel *> enqueuedVoxel;
	// Put first voxel to the queue and set is as analyzed;
	enqueuedVoxel.push(firstVoxel);
	while (!enqueuedVoxel.empty()) {
		FFVoxel *currentVoxel = enqueuedVoxel.front();
		enqueuedVoxel.pop();
		if (currentVoxel->analyzed)
			continue;

		currentVoxel->analyzed = true;
		object.putVoxel(currentVoxel);
		// Analyze all neighbours
		for (int i = 0; i < currentVoxel->neighboursNumber; ++i) {
			FFVoxel *neighbour = currentVoxel->neighbours[i];
			Vec3f neighbourCoords = Vec3f(neighbour->coords.x, neighbour->coords.y, neighbour->coords.z);
			if (neighbour->analyzed || plane(neighbourCoords) < 0.0f)
				continue;

			enqueuedVoxel.push(neighbour);
		}
	}

	return true;
}

}
