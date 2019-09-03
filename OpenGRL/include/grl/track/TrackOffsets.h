#pragma once

#include <grl/track/Track.h>

namespace grl {

/**
 * Track, which is being represented by the sequence of the transformations.
 * Each element in the track is representing the offset from the last added
 * point, so applying each transformation to the starting point will lead to
 * the next points of the track and, finally, to the end.
 */
class TrackOffsets : public Track<OrientedTransformation>
{
    using super = Track<OrientedTransformation>;
    /* Utilities for track manipulation */
    friend class TrackTools;

public:
    /**
     * Default constructor initializing all fields.
     */
    TrackOffsets();

    /**
     * Initialize the track already with the starting point.
     *
     * @param startingPoint Starting point of the track - it will be treated as
     * the beginning of the track and all transformations will apply to that
     * point.
     */
    TrackOffsets(Vec3f startingPoint);

    /**
     * Get the i-th point location of the track in the 3D space. It returns
     * starting point with all applied transformations.
     *
     * @param i Index of point which in the order it was added to the track and
     *          which representation should be returned.
     * @returns point in the world space that is part of the track and was added as i-th element.
     */
    virtual Vec3f getPoint(size_t i) const override;

    /**
     * Set the new starting point of the track. First offset will be in relation
     * to that point and won't change.
     *
     * @param newStartingPoint New starting point for the track.
     */
    void setStartingPoint(Vec3f newStartingPoint);

    /**
     * Clear whole track. After this function ends, the track will be in the
     * same state as after executing default constructor.
     */
    virtual void clear() override;

private:
    /* Beginning of the track */
    Vec3f _startingPoint;
    /* Last point that was added to the track, used for convenience */
    Vec3f _lastAddedPoint;
    /* Indicates if the _startingPoint exists. */
    bool _firstAdded;

    /**
     * Append new point to the track. It is calculating the offset from the last
     * added point and adding it to vector of elements.
     */
    virtual void updateTrackWithPoint(Vec3f newPoint) override;

    virtual void updateLengthAfterAddingPoint(Vec3f newPoint) override;
};

}
