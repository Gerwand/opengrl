#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#include <vector>
#include <queue>
#include <algorithm>

#include <opencv/cv.hpp>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>

#include <grl/utils/MathUtils.h>

typedef std::vector<cv::Point3i> Points3i;

namespace grl {

template<typename T>
struct Voxel
{
	typedef std::vector<Voxel *> VoxelPtrs;

	cv::Point coords;
	T value;

	bool analysed = false;
	VoxelPtrs neighbours;

	Voxel(int x = 0, int y = 0, T z = 0) : coords(x, y), value(z)
	{
		neighbours.reserve(8);
	}
	void set(int x, int y, T z)
	{
		coords.x = x;
		coords.y = y;
		value = z;
		analysed = false;
	}
};

template<typename T>
struct Object
{
	typedef std::vector<Voxel<T> *> Voxels;
	typedef std::vector<cv::Point> Points;

	Voxels voxels;				// Points, that are creating object (x, y, z)
	Points points;
	std::vector<Points> contours;

	cv::Rect box;				// Smallest box around the object (x, y, z)
	cv::RotatedRect ellipse;
	cv::Point2f center;			// Center of the box

	cv::Point2i massCenter;		// Geometric/mass center of the object

	T area = 0;					// Total area of the object
	T perimeter = 0;			// Perimeter of the object
	int diameter = 0;			// Diameter of the object - biggest possible
	T meanValue = 0;			// Mean value of the whole object

	double circularity = 0;
	double roundness = 0;
	double angle = 0;
	double aspectRatio = 0;

	cv::Mat segmentImage;
	cv::Mat contourImage;

	Object() : box(INT_MAX, INT_MAX, -1, -1) {}

	void updateCoords(Voxel<T> *vox);
	void calculateProperties(double tolerance);

	void drawOnImage(cv::Mat& mat)
	{
		for (auto it = points.begin(); it != points.end(); ++it)
			mat.at<uchar>((*it)) = 255;
	}
};

template<typename T>
void
Object<T>::updateCoords(Voxel<T> *vox)
{
	voxels.push_back(vox);
	points.push_back(vox->coords);
	area++;

	// Update integral of the mass center
	massCenter.x += vox->coords.x;
	massCenter.y += vox->coords.y;
}

template<typename T>
inline std::ostream&
operator<<(std::ostream& stream, const Object<T>& object)
{
	return stream << "[center = " << object.center
		<< ", massCenter = " << object.massCenter
		<< ", box = " << object.box
		<< ", area = " << object.area
		<< ", perimeter = " << object.perimeter
		<< ", diameter = " << object.diameter
		<< ", value = " << object.meanValue
		<< ", circularity = " << object.circularity
		<< ", roundness = " << object.roundness
		<< ", aspectRatio = " << object.aspectRatio
		<< "]";
}

template<typename T>
inline void
Object<T>::calculateProperties(double tolerance)
{
	massCenter /= area;
	box = cv::boundingRect(points);

	ellipse = fitEllipse(points);
	angle = ellipse.angle;
	aspectRatio = ellipse.size.width / ellipse.size.height;

	// Approximate diameter by size of the box
	diameter = std::max(box.width, box.height);
	roundness = 4 * area / (M_PI * diameter * diameter);

	segmentImage = cv::Mat::zeros(box.size(), CV_8UC1);
	for (auto it = points.begin(); it != points.end(); ++it) {
		segmentImage.at<uchar>(cv::Point(it->x - box.x, it->y - box.y)) = 255;
	}

	std::vector<cv::Vec4i> hierarchy;

	cv::Canny(segmentImage, contourImage, 100, 200);
	for (auto it = contourImage.begin<uchar>(); it != contourImage.end<uchar>(); ++it)
		if (*it != 0)
			perimeter++;
	circularity = 4 * M_PI * area / (perimeter * perimeter);
}

template <typename T>
class FloodFill
{
public:
	typedef std::vector<Voxel<T> > Voxels;
	typedef std::vector<Object<T> > Objects;

	FloodFill(int minSize = 0, double tolerance = 1);
	void setup(int minSize, double tolerance);

	void getObjects(const cv::Mat& image, Objects& objects);

private:
	int _minSize;
	double _tolerance;

	Voxels _voxels;

	void extractObject(Voxel<T>& vox, Object<T>& object);
	void convertToPoints(const cv::Mat& image);
};

template <typename T>
inline
FloodFill<T>::FloodFill(int minSize, double tolerance)
{
	_minSize = minSize;
	_tolerance = tolerance;
}

template <typename T>
inline void
FloodFill<T>::setup(int minSize, double tolerance)
{
	_minSize = minSize;
	_tolerance = tolerance;
}

template <typename T> void
FloodFill<T>::convertToPoints(const cv::Mat& image)
{
	auto it = image.begin<T>();

	_voxels.resize(image.cols * image.rows);
	auto itv = _voxels.begin();
	// Create points
	for (int y = 0; y < image.rows; ++y)
		for (int x = 0; x < image.cols; ++x, ++it, ++itv)
			itv->set(x, y, *it);

	// Fill neighbours
	auto itPt = _voxels.begin();

	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x, ++itPt) {
			Voxel<T>& vox = *itPt;
			if (vox.value == 0)
				continue;

			int xmin = clampMin(x - 1, 0),
				xmax = clampMax(x + 1, image.cols - 1),
				ymin = clampMin(y - 1, 0),
				ymax = clampMax(y + 1, image.rows - 1);

			vox.neighbours.clear();

			for (int yl = ymin; yl <= ymax; ++yl) {
				for (int xl = xmin; xl <= xmax; ++xl) {

					if (xl == x && yl == y)
						continue;

					int index = yl*image.cols + xl;

					Voxel<T> *neigh = &_voxels[index];
					if (neigh->value != 0) {
						double dist = abs((double)vox.value - (double)neigh->value);
						if (dist <= _tolerance)
							vox.neighbours.push_back(neigh);
					}
				}
			}
		}
	}
}

template <typename T> void
FloodFill<T>::getObjects(const cv::Mat& image, Objects& objects)
{
	convertToPoints(image);

	int i = 0;
	for (auto it = _voxels.begin(); it != _voxels.end(); ++it) {
		Voxel<T>& vox = *it;
		Object<T> obj;
		if (!vox.analysed) {
			vox.analysed = true;
			if (vox.value == 0)
				continue;
			if (vox.neighbours.empty())
				continue;

			extractObject(vox, obj);
			if (obj.area > _minSize) {
				obj.calculateProperties(_tolerance);
				objects.push_back(obj);
			}
		}
	}
}

template <typename T> void
FloodFill<T>::extractObject(Voxel<T>& vox, Object<T>& object)
{
	std::queue<Voxel<T> *> enqueuedVoxels;
	Voxel<T>::VoxelPtrs::iterator itNeigh;
	Voxel<T> *current = &vox;

	while (true) {
		object.meanValue += current->value;
		object.updateCoords(current);

		itNeigh = current->neighbours.begin();
		for (; itNeigh != current->neighbours.end(); ++itNeigh) {
			Voxel<T> *neighbour = *itNeigh;
			if (neighbour->analysed)
				continue;

			neighbour->analysed = true;
			enqueuedVoxels.push(neighbour);
		}
		if (!enqueuedVoxels.empty()) {
			current = enqueuedVoxels.front();
			enqueuedVoxels.pop();
		} else {
			break;
		}
	}
}

}