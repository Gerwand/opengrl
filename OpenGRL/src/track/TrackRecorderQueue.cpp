#include <grl/track/TrackRecorderQueue.h>

namespace grl {

TrackRecorderQueue::TrackRecorderQueue()
{

}

TrackRecorderQueue::TrackRecorderQueue(float minOffset, uint8_t skip, size_t bufferLen, size_t maxTrackSize)
    : TrackRecorder(minOffset, skip, bufferLen, maxTrackSize)
{

}

TrackRecorder::RecorderState TrackRecorderQueue::addPoint(Vec3f point)
{
    bufferNextPoint(point);

    // The point is added to the track every frames/points number equal to the _step
    if (_currentFrame++ != _skip)
        return grlRecorderPointBuffered;

    // If it is _step frame, reset the counter and try to push the point
    _currentFrame = 0;

    // Get the point using median filter to reduce the noise
    const IndexedFloat medianDistance = getMedianFromBuffer();

    // If the moved distance was too small, do not add it to the track
    if (medianDistance.second < _minOffset)
        return grlRecorderPointSkipped;

    // Finally, add the point to the track
    const Vec3f &midPoint = _buffer[medianDistance.first];
    enqueuePoint(midPoint);
    _lastTrackPoint = midPoint;

    return grlRecorderPointAdded;
}

size_t TrackRecorderQueue::getTrackSize()
{
    return _buffer.size();
}

void TrackRecorderQueue::clear()
{
    _trackPointsQueue.clear();
    TrackRecorder::clear();
}

void TrackRecorderQueue::enqueuePoint(Vec3f point)
{
    // If already buffered max points, remove first one (oldest one) to make the place for the newer point
    if (_trackPointsQueue.size() == _maxTrackSize)
        _trackPointsQueue.pop_front();

    // Add point to the buffer - it will be used for median filter
    _trackPointsQueue.push_back(point);
}

}