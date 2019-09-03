#pragma once

#include <grl/utils/MathUtils.h>

#include <vector>

namespace grl {

/**
 * Base class for all representations of the tracks. It is meant to be
 * represented by the vector of T elements and is implementation specific. It
 * is just an interface.
 *
 * @tparam T the type of the elements which are representing the track.
 */
template <typename T>
class Track
{
public:
    /**
     * Get the i-th point location of the track in the 3D space. It may not be
     * the element stored in the class and it sometimes may be calculated, for
     * example if the track is stored as an offsets instead of simply points.
     * Returned point should lay in the world space and be part of the track.
     *
     * @param i Index of point which in the order it was added to the track and
     *          which representation should be returned.
     * @returns Point in the world space that is part of the track and was added as i-th element.
     */
    virtual Vec3f getPoint(size_t i) const = 0;

    /**
     * Get the i-th point location of the track in the 3D space. Simple wrapper
     * over getPoint() function.
     *
     * @param i Index of point which in the order it was added to the track and
     *          which representation should be returned.
     * @returns Point in the world space that is part of the track and was added as i-th element.
     */
    Vec3f operator()(size_t i) const;

    /**
     * Get the i-th element of the vector of elements representing tracks. It may
     * not be the same element that was added to the track as i-th point, it is
     * implementation specific.
     *
     * @param i Index of the element that should be returned.
     * @returns reference to the i-th element of the implementation specific track represenation
     */
    T & getElement(size_t i);

    /**
     * Get the i-th element of the vector of elements representing tracks. It may
     * not be the same element that was added to the track as i-th point, it is
     * implementation specific.
     *
     * @param i Index of the element that should be returned.
     * @returns reference to the i-th element of the implementation specific track represenation
     */
    const T & getElement(size_t i) const;

    /**
     * Get the i-th element of the vector of elements representing tracks. It may
     * not be the same element that was added to the track as i-th point, it is
     * implementation specific.
     *
     * @param i Index of the element that should be returned.
     * @returns reference to the i-th element of the implementation specific track represenation
     */
    T & operator[](size_t i);

    /**
     * Get the i-th element of the vector of elements representing tracks. It may
     * not be the same element that was added to the track as i-th point, it is
     * implementation specific.
     *
     * @param i Index of the element that should be returned.
     * @returns reference to the i-th element of the implementation specific track represenation
     */
    const T & operator[](size_t i) const;

    /**
     * Add point to the track. The coordinates of the point should be in the
     * world space.
     *
     * @param point Point that should be added to the track.
     * @returns reference to the i-th element of the implementation specific track represenation.
     */
    void addPoint(Vec3f point);

    /**
     * Clear whole track. After this function ends, the track will be in the
     * same state as after executing default constructor. If the implementation
     * needs to do additional cleanup, it should override this function and call
     * the parent's one.
     */
    virtual void clear();

    /**
     * Get the length of the whole track.
     *
     * @returns length of the whole track
     */
    float getTrackLength() const;

    /**
     * Get number of points that are creating the track.
     *
     * @returns number of points which are creating the track
     */
    size_t getPointsCount() const;

    /**
     * For convenience, return the last point in the 3D world space of the track.
     *
     * @returns last point in the track.
     */
    Vec3f getLastPoint() const;

    /**
     * Check if the track is empty - it means that it has no any points inside.
     */
    bool isEmpty() const;

    /**
     * Iterators for quickly accessing the track elements.
     */
    typename std::vector<T>::iterator begin();

    /**
     * Iterators for quickly accessing the track elements.
     */
    typename std::vector<T>::iterator end();

    typename const std::vector<T> & getVectorOfElements() const { return _trackElements; }

protected:
    /**
     * The track is represented by the vector of T elements and their
     * interpretation is implementation specific.
     */
    std::vector<T> _trackElements;
    /* Total length of the track. */
    float _trackLength = 0;
    /* Number of points inside the track */
    size_t _trackPointsNum = 0;

    /**
     * Append new point to the track. The class which is implementing the
     * interface should convert the input point from world space to the
     * appropriate resource of T type and add it to the _trackElements array if
     * needed.
     * This function is being called by addPoint().
     *
     * @param newPoint Point in 3D space that should be added to the track.
     */
    virtual void updateTrackWithPoint(Vec3f newPoint) = 0;

    /**
     * Update length of the track after the point was already added. For
     * convenience the point in 3D space that was added is being passed as an
     * argument.
     * This function is being called by addPoint() after updateTrackWithPoint is
     * being added.
     *
     * @param newPoint Point in 3D space that was added to the track.
     */
    virtual void updateLengthAfterAddingPoint(Vec3f newPoint) = 0;
};

template <typename T>
Vec3f Track<T>::operator()(size_t i) const
{
    return getPoint(i);
}

template <typename T>
T & Track<T>::getElement(size_t i)
{
    return _trackElements[i];
}

template <typename T>
const T & Track<T>::getElement(size_t i) const
{
    return _trackElements[i];
}

template <typename T>
T & Track<T>::operator[](size_t i)
{
    return getElement(i);
}

template <typename T>
const T & Track<T>::operator[](size_t i) const
{
    return getElement(i);
}

template <typename T>
float Track<T>::getTrackLength() const
{
    return _trackLength;
}

template <typename T>
void Track<T>::clear()
{
    _trackElements.clear();
    _trackLength = 0;
    _trackPointsNum = 0;
}

template <typename T>
void Track<T>::addPoint(Vec3f point)
{
    updateTrackWithPoint(point);
    updateLengthAfterAddingPoint(point);
    ++_trackPointsNum;
}

template <typename T>
size_t Track<T>::getPointsCount() const
{
    return _trackPointsNum;
}

template <typename T>
Vec3f Track<T>::getLastPoint() const
{
    return getPoint(_trackPointsNum - 1);
}

template <typename T>
typename std::vector<T>::iterator Track<T>::begin()
{
    return _trackElements.begin();
}

template <typename T>
typename std::vector<T>::iterator Track<T>::end()
{
    return _trackElements.end();
}

template <typename T>
bool Track<T>::isEmpty() const
{
    return _trackPointsNum == 0;
}

}
