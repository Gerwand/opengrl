#include <grl/track/TrackPoints.h>

namespace grl {

TrackPoints::TrackPoints()
{
}

Vec3f TrackPoints::getPoint(size_t i) const
{
    return _trackElements[i];
}

void TrackPoints::updateTrackWithPoint(Vec3f newPoint)
{
    _trackElements.push_back(newPoint);
}

void TrackPoints::updateLengthAfterAddingPoint(Vec3f newPoint)
{
    // To determine track length, we need at least 2 points
    size_t numberOfTrackElements = _trackElements.size();
    if (numberOfTrackElements < 2)
        return;

    // As the point is already put into the track, we need to calculate distance between last
    // element and the second last
    _trackLength += grl::Vec3f::distance(newPoint,
                                         _trackElements.at(numberOfTrackElements - 2));
}

}
