#pragma once

#include <grl/track/TrackPoints.h>

#include <deque>

namespace grl {

/**
 * TrackRecorder is a class that allows to save input points with the noise
 * reduction and skipping of the irrevelant transformations.
 * The point of this tool is to do not record the track in terms of it's changes
 * over time, but rather to save a shape of a track.
 * The default behavior can be changed using the configuration parameters
 * provided in the constructor.
 * To provide noise reduction, the FIFO queue can be used. It is buffering the
 * last few points (their number is being configured and must be at least 1),
 * from which the median of the offsets in relation to the last point is being
 * calculated, for each few points added.
  */
class TrackRecorder
{
public:
    /**
     * Actions that could take a part upon adding point to a recorder.
     */
    enum RecorderState  {
        /* The point was successfully added to a track. Will occur every step points. */
        grlRecorderPointAdded,
        /* The point was buffered, but wasn't added to a track. */
        grlRecorderPointBuffered,
        /*
         * Either the track has achieved it's max allowed size or the offset median
         * from the buffer in relation to the last point wasn't big enough.
         */
        grlRecorderPointSkipped,
    };

    /**
     * Default constructor, doing nothing.
     */
    TrackRecorder();

    /**
     * Constructor, which is initializing the recorder with the given parameters.
     *
     * @param minOffset Minimum offset of the point that is considered to be
     *        added to the track. If the bufferLen > 1, the offset of the median
     *        point is considered.
     * @param skip Number of points that must be added to the recorder, before
     *        next one will be considered to be added to the track.
     * @param bufferLen Max number of points that will be buffered and from
     *        which the median offset will be calculated. Increasing this
     *        parameter will reduce noise but increase inertia.
     * @param maxTrackSize Maximum size of the track. If the number of points
     *        added to a track will exceed this value, all next points will be
     *        skipped.
     */
    TrackRecorder(float minOffset, uint8_t skip, size_t bufferLen, size_t maxTrackSize = SIZE_MAX);

    /**
     * Clear the track saved in the recorder and reset recorder to the initial
     * state.
     */
    void clear();

    /**
     * Get the reference to the recorded track.
     *
     * @returns reference to the recorded track.
     */
    const TrackPoints & getTrack() const;

    /**
     * Add point to the recorer. This function will return flag which will
     * indicate what happened inside:
     * - grlRecorderPointAdded - the point was added to a track.
     * - grlRecorderPointBuffered - the point wasn't added, just buffered.
     * - grlRecorderPointSkipped - the point wasn't added, either the offset of
     *   the median point was too small, or the track has achieved it's maximum
     *   size.
     *
     * @returns flag what happened with the point.
     */
    virtual RecorderState addPoint(Vec3f point);

protected:
    /* Helping type to associate float value with the index for sorting. */
    using IndexedFloat = std::pair<size_t, float>;

    // Buffers
    /* Buffer for calculating the median from the _maxBufferLen points. */
    std::deque<Vec3f> _buffer;
    /* Saved track. */
    TrackPoints _track;

    // Configuration
    /* Minimum offset from the last point to consider the new point as a new track element. */
    float _minOffset;
    /* Maxiumum allowed buffer length. */
    size_t _maxBufferLen;
    /* Maximum allowed number of segments inside the track. */
    size_t _maxTrackSize;
    /* Number of points that should be skipped before the median of _maxBufferLen is added to the track. */
    uint8_t _skip;
    Vec3f _lastTrackPoint;

    /* Internal counter for number of points already buffered. */
    int _currentFrame = 0;

    // Internal functions

    /**
     * Add the point to the buffer and remove oldest one if the buffer is full.
     */
    void bufferNextPoint(Vec3f point);

    /**
     * Calculate median point from all points in the buffer.
     *
     * @returns pair, where first is the index of the median element in the buffer,
     * the second is the offset from the last added point of the median element.
     */
    IndexedFloat getMedianFromBuffer();
};

}
