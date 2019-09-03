#pragma once

#include <grl/track/TrackRecorder.h>
#include <grl/track/TrackOffsets.h>

namespace grl {

/**
 * This implementation of the recorder is meant to be used as a buffer for
 * all points with possibility to quickly remove oldest point whenever newer one
 * must be added.
 */
class TrackRecorderQueue : private TrackRecorder
{
public:

    /**
     * Default constructor, doing nothing.
     */
    TrackRecorderQueue();

    /**
     * Constructor, which is initializing the recorder with the given parameters.
     *
     * @param minOffset Minimum offset of the point that is considered to be
     *        added to the track. If the bufferLen > 1, the offset of the median
     *        point is considered.
     * @param step How many frames will be buffered until the median from points
                   is considered to be added to a track.
     * @param bufferLen Max number of points that can be saved in the recorder.
     * @param maxTrackSize Max size of the track queue.
     */
    TrackRecorderQueue(float minOffset, uint8_t skip, size_t bufferLen, size_t maxTrackSize);

    /**
     * Clear the track saved in the recorder and reset recorder to the initial
     * state.
     */
    void clear();

    /**
     * Convert all currently buffered points into the Track<T>.
     *
     * @param[out] track to which the buffer should be saved.
     * @tparam T the type of the elements which are representing the track.
     */
    template <typename T>
    void getTrack(Track<T> &track) const;

    /**
     * Add point to the recorder. This function will return flag which will
     * indicate what happened inside:
     * - grlRecorderPointBuffered - the point was successfully added to a buffer.
     * - grlRecorderPointAdded - the point was successfully added to a queue.
     * - grlRecorderPointSkipped - the point was skipped, because the distance wasn't big enough.
     *
     * @returns flag what happened with the point.
     */
    RecorderState addPoint(Vec3f point) override;

    /**
     * Return number of points currently hold in the queue.
     *
     * @returns number of points in the queue
     */
    size_t getTrackSize();

private:
    // For faster remove of the last/first points, use the dequeue instead
    // of TrackPoints, which is using the vector inside.
    std::deque<Vec3f> _trackPointsQueue;

    void enqueuePoint(Vec3f point);
};

template <typename T>
void TrackRecorderQueue::getTrack(Track<T> &track) const
{
    track.clear();

    for (auto it = _trackPointsQueue.cbegin(); it != _trackPointsQueue.cend(); ++it)
        track.addPoint(*it);
}

}
