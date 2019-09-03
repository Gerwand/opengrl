#include <grl/track/TrackRecorder.h>

namespace grl {

using IndexedFloat = std::pair<size_t, float>;

TrackRecorder::TrackRecorder()
{

}

TrackRecorder::TrackRecorder(float minOffset, uint8_t skip, size_t bufferLen, size_t maxTrackSize)
{
    assert(bufferLen > 0 && "Buffer cannot have 0 size");

    _minOffset = minOffset;
    _skip = skip;
    _maxBufferLen = bufferLen;
    _maxTrackSize = maxTrackSize;
    _lastTrackPoint = Vec3f(0.0f, 0.0f, 0.0f);
}

void TrackRecorder::clear()
{
    _buffer.clear();
    _track.clear();
    _currentFrame = 0;
    _lastTrackPoint = Vec3f(0.0f, 0.0f, 0.0f);
}

const TrackPoints & TrackRecorder::getTrack() const
{
    return _track;
}

void TrackRecorder::bufferNextPoint(Vec3f point)
{
    // If already buffered max points, remove first one (oldest one) to make the place for the newer point
    if (_buffer.size() == _maxBufferLen)
        _buffer.pop_front();
    // Add point to the buffer - it will be used for median filter
    _buffer.push_back(point);
}

IndexedFloat TrackRecorder::getMedianFromBuffer()
{
    // Convert all of the points to the transformations in relation to the last point
    std::vector<IndexedFloat> bufferDistances;
    bufferDistances.reserve(_buffer.size());

    // Keep all transformations indexed, to allow them to be sorted
    size_t index = 0;
    for (auto itBuffer = _buffer.cbegin(); itBuffer != _buffer.cend(); ++itBuffer, ++index) {
        IndexedFloat bufDistance;
        bufDistance.first = index;
        bufDistance.second = Vec3f::distance(*itBuffer, _lastTrackPoint);
        bufferDistances.push_back(bufDistance);
    }

    // Sort all distances to get the median
    std::sort(bufferDistances.begin(), bufferDistances.end(), [](IndexedFloat a, IndexedFloat b) {
            return a.second < b.second;
    });

    // Get the median element from the buffer
    size_t midIndex = bufferDistances.size()/2;
    return bufferDistances[midIndex];
}

TrackRecorder::RecorderState TrackRecorder::addPoint(Vec3f point)
{
    // Check if number of segments inside the track is not greater than the limit.
    if (_track.getPointsCount() == _maxTrackSize)
        return grlRecorderPointSkipped;

    bufferNextPoint(point);

    // The step frames must be skipped before the point will be added
    if (_currentFrame++ != _skip)
        return grlRecorderPointBuffered;

    // If it is _skip + 1 frame, reset the counter and try to push the point
    _currentFrame = 0;

    // Get the point using median filter to reduce the noise
    const IndexedFloat medianDistance = getMedianFromBuffer();

    // If the moved distance was too small, do not add it to the track, unless
    // the track is empty.
    if (!_track.isEmpty() && medianDistance.second < _minOffset)
        return grlRecorderPointSkipped;

    // Finally, add the point to the track
    const Vec3f &midPoint = _buffer[medianDistance.first];
    _track.addPoint(midPoint);
    _lastTrackPoint = midPoint;

    return grlRecorderPointAdded;
}

}