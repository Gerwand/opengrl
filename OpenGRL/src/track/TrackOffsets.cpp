#include <grl/track/TrackOffsets.h>

namespace grl {

TrackOffsets::TrackOffsets()
    : _firstAdded(false)
{
}

TrackOffsets::TrackOffsets(grl::Vec3f startingPoint)
    : _startingPoint(startingPoint)
    , _lastAddedPoint(startingPoint)
    , _firstAdded(true)
{
}

void TrackOffsets::clear()
{
    super::clear();

    _startingPoint = Vec3f(0.0f, 0.0f, 0.0f);
    _lastAddedPoint = Vec3f(0.0f, 0.0f, 0.0f);
    _firstAdded = false;
}

Vec3f TrackOffsets::getPoint(size_t i) const
{
    // Checking for size + 1, because first point is stored at _startingPoint and
    // next elements are offsets from this point.
    assert(i < _trackPointsNum);

    // If that's the last point, return last added point to be a bit faster
    if (i == _trackElements.size())
        return _lastAddedPoint;

    Vec3f finalPoint = _startingPoint;
    auto it = _trackElements.begin();
    for (int n = 0; n < i; ++n, ++it)
        finalPoint += it->getTransformationVector();

    return finalPoint;
}

void TrackOffsets::updateTrackWithPoint(Vec3f newPoint)
{
    if (!_firstAdded) {
        // If the starting point wasn't set, set it now
        _startingPoint = newPoint;
        _firstAdded = true;
    } else {
        // If the first points is already existing, set it now
        Vec3f transformationVector = newPoint - _lastAddedPoint;
        OrientedTransformation transformation(transformationVector);

        _trackElements.push_back(transformation);
    }
    // Save last added point for convenience to avoid recalculation of the tail
    // each time the new point is being added.
    _lastAddedPoint = newPoint;
}

void TrackOffsets::updateLengthAfterAddingPoint(Vec3f newPoint)
{
    // We need at least _startingPoint and single oriented transformation to
    // determine length of the track
    if (_trackElements.empty())
        return;

    _trackLength += _trackElements.back().getDistance();
}

void TrackOffsets::setStartingPoint(Vec3f newStartingPoint)
{
    _startingPoint = newStartingPoint;
    if (!_firstAdded) {
        _lastAddedPoint = newStartingPoint;
        _firstAdded = true;
        ++_trackPointsNum;
    }
}

}
