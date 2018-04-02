#include "GestureExtractor.h"

namespace grl {

void
Object::putVoxel(Voxel *voxel)
{
	voxels.push_back(voxel);
	// Recalcuate size
	size = voxels.size();

	// Recalculate bounding box (width and height)
	// Width
	if (voxel->coords.x < boundingBox.x) {
		boundingBox.x = voxel->coords.x;
		boundingBox.width = _maxX - boundingBox.x + 1;
	}
	if (voxel->coords.x > _maxX) {
		_maxX = voxel->coords.x;
		boundingBox.width = _maxX - boundingBox.x + 1;
	}
	// Height
	if (voxel->coords.y < boundingBox.y) {
		boundingBox.y = voxel->coords.y;
		boundingBox.height = _maxY - boundingBox.y + 1;
	} 
	if (voxel->coords.y > _maxY) {
		_maxY = voxel->coords.y;
		boundingBox.height = _maxY - boundingBox.y + 1;
	}

	// Recalculate min and max val
	if (voxel->coords.z < minVal)
		minVal = voxel->coords.z;
	if (voxel->coords.z > maxVal)
		maxVal = voxel->coords.z;
}

void 
Object::reset()
{
    size = 0;
    minVal = INT_MAX;
    maxVal = INT_MIN;
    accuracy = 0;
    boundingBox = cv::Rect(INT_MAX, INT_MAX, 0, 0);
    _maxX = INT_MIN;
    _maxY = INT_MIN;
    voxels.clear();
}

void
Object::generateImage()
{
	image = cv::Mat::zeros(boundingBox.height, boundingBox.width, CV_16UC1);
	uint16_t *imgData = reinterpret_cast<uint16_t *>(image.data);

	auto itVoxel = voxels.begin();
	for (; itVoxel != voxels.end(); ++itVoxel) {
		const Voxel *voxel = *itVoxel;
		int pixelIndex = (voxel->coords.x - boundingBox.x) + (voxel->coords.y - boundingBox.y)*boundingBox.width;
		imgData[pixelIndex] = static_cast<uint16_t>(voxel->coords.z);
	}
}

}
