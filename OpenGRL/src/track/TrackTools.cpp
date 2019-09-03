#include <grl/track/TrackTools.h>

namespace grl {

void TrackTools::translateTrackPoints(TrackPoints &points, Vec3f translationVector)
{
    for (auto it = points._trackElements.begin(); it != points._trackElements.cend(); ++it)
        *it += translationVector;
}

void TrackTools::translateTrackOffsets(TrackOffsets &offsets, Vec3f translationVector)
{
    offsets.setStartingPoint(offsets._startingPoint + translationVector);
}

void TrackTools::trackOffsetsToTrackPoints(const TrackOffsets &sourceOffsets,
                                           TrackPoints &destinationPoints,
                                           bool excludeStart)
{
    destinationPoints.clear();
    // If there wasn't even the first point added, there is nothing to convert
    if (!sourceOffsets._firstAdded)
        return;

    Vec3f currentPoint = excludeStart ? Vec3f(0.0f, 0.0f, 0.0f) : sourceOffsets._startingPoint;

    destinationPoints._trackElements.push_back(currentPoint);
    for (auto it = sourceOffsets._trackElements.cbegin(); it != sourceOffsets._trackElements.end(); ++it) {
        currentPoint += it->getTransformationVector();
        destinationPoints._trackElements.push_back(currentPoint);
    }
}

void TrackTools::trackPointsToTrackOffsets(const TrackPoints &sourcePoints,
                                      TrackOffsets &destinationOffsets)
{
    for (auto it = sourcePoints._trackElements.cbegin(); it != sourcePoints._trackElements.end(); ++it)
        destinationOffsets.addPoint(*it);
}

}
