#include <grl/track/TrackTools.h>

#include <fstream>
#include <iostream>

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

bool TrackTools::saveTrackToFile(const TrackPoints &track,
                                 const std::string &fileName)
{
    std::ofstream file(fileName, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open())
        return false;

    file << 'N' << track.getPointsCount() << std::endl;
    for (size_t i = 0; i < track.getPointsCount(); ++i) {
        const Vec3f &point = track[i];
        file << point.x << ";" << point.y << ";" << point.z << std::endl;
    }
    return true;
}

bool TrackTools::loadTrackFromFile(TrackPoints &track,
                                   const std::string &fileName)
{
    char cmd;
    size_t npoints;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        return false;
    }

    file >> cmd >> npoints;
    if (cmd != 'N') {
        std::cerr << "Invalid file format" << std::endl;
        return false;
    }

    for (size_t i = 0; i < npoints; ++i) {
        Vec3f point;
        char skip;
        file >> point.x >> skip >> point.y >> skip >> point.z;
        track.addPoint(point);
    }

    return true;
}

}
