#pragma once

#include <grl/track/Track.h>

namespace grl {

/**
 * Track, which is being represented by the points in the 3D space.
 */
class TrackPoints : public Track<Vec3f>
{
    /* Utilities for track manipulation */
    friend class TrackTools;

public:
    /**
     * Default constructor, doing nothing.
     */
    TrackPoints();

    /**
     * Get the i-th point location of the track in the 3D space. It gives the
     * same result as the getElement(i) as the vector of elements is simply
     * storing the points in the world space.
     *
     * @param i Index of point which in the order it was added to the track and
     *          which representation should be returned.
     * @returns Point in the world space that is part of the track and was added as i-th element.
     */
    virtual Vec3f getPoint(size_t i) const override;

private:
    /**
     * Append new point to the track. It is simply putting the newPoint to the
     * vector of elements.
     */
    virtual void updateTrackWithPoint(Vec3f newPoint) override;

    virtual void updateLengthAfterAddingPoint(Vec3f newPoint) override;
};

}
