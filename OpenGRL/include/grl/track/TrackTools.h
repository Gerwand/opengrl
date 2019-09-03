#pragma once

#include <grl/track/TrackOffsets.h>
#include <grl/track/TrackPoints.h>

namespace grl {

class TrackTools {
public:
    static void translateTrackPoints(TrackPoints &points, Vec3f translationVector);
    static void translateTrackOffsets(TrackOffsets &offsets, Vec3f translationVector);
    static void trackOffsetsToTrackPoints(const TrackOffsets &sourceOffsets,
                                          TrackPoints &destinationPoints,
                                          bool excludeStart = false);
    static void trackPointsToTrackOffsets(const TrackPoints &sourcePoints,
                                          TrackOffsets &destinationOffsets);
    static bool saveTrackToFile(const TrackPoints &track,
                                const std::string &fileName);
    static bool loadTrackFromFile(TrackPoints &track,
                                  const std::string &fileName);
};

}
