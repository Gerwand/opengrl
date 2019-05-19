#pragma once

#include <grl/track/GestureTracker.h>

#include <deque>

namespace grl {

struct DiscretizedTrackerConfig : TrackerConfig
{
    float discreteDelta; // Distance between next points when discretizing track
    int maxCompareSegmentsDiff; // Segments difference for tacks comparison
};

class DiscretizedGestureTracker : public GestureTracker
{
public:
    virtual bool init(std::unique_ptr<TrackerConfig> config) override;

    virtual void processTrack(const TrackOffsets &offsets, TrackPoints &processed) override;
private:
    DiscretizedTrackerConfig *_discretizedConfig;

    virtual float compareTracks(const TrackPoints &first, const TrackPoints &second) override;
    virtual void processTrack(const std::deque<OrientedTransformation> &offsets, TrackPoints &processed) override;

    template <class ForwardIt>
    void discretizeTrack(ForwardIt itBegin, ForwardIt itEnd, TrackPoints &processed);
};

inline void 
DiscretizedGestureTracker::processTrack(const TrackOffsets &offsets, TrackPoints &processed)
{
    discretizeTrack(offsets.transformations.cbegin(), offsets.transformations.cend(), processed);
}

inline void
DiscretizedGestureTracker::processTrack(const std::deque<OrientedTransformation> &offsets, TrackPoints &processed)
{
    discretizeTrack(offsets.cbegin(), offsets.cend(), processed);
}


template <class ForwardIt>
void
DiscretizedGestureTracker::discretizeTrack(ForwardIt itBegin, ForwardIt itEnd, TrackPoints &processed)
{
    // Discretize the offsets without including the starting point, so every track will begin from the same point.
    std::vector<Vec3f> &discretizedPoints = processed.points;
    discretizedPoints.clear();

    // Add starting point - the beginning of the coordinate system
    Vec3f lastPoint = Vec3f(0.0f, 0.0f, 0.0f);
    discretizedPoints.push_back(lastPoint);

    Vec3f totalOffset = Vec3f(0.0f, 0.0f, 0.0f);
    float totalLength = 0.0f;
    float delta = _discretizedConfig->discreteDelta;
    auto itOffset = itBegin;
    for (; itOffset != itEnd; ++itOffset) {
        float len = itOffset->getDistance();

        // If it is next point in a sequence, calulace part of the delta that must be taken
        float deltaLeft = delta - totalLength;
        if (len < deltaLeft) {
            // If the section is smaller than rest of the delta, append current section to next offset
            totalOffset += itOffset->getTransformationVector();
            totalLength += len;
            continue;
        }

        // If the section length is greater or equal to delta, add deltaLeft portion of it to the offset
        len -= deltaLeft;
        totalOffset += itOffset->getUnitVector() * deltaLeft;
        lastPoint += totalOffset;
        discretizedPoints.push_back(lastPoint);

        // Discretize, until the delta is reached
        int wholeSegments = static_cast<int>(len /  delta);
        Vec3f deltaOffset = itOffset->getUnitVector() *  delta;
        for (int i = 0; i < wholeSegments; ++i) {
            lastPoint += deltaOffset;
            discretizedPoints.push_back(lastPoint);
        }
        len -= wholeSegments *  delta;

        // Now, add the rest of the offset
        totalOffset = len * itOffset->getUnitVector();
        totalLength = len;
    }

    // Now, add remaining part of the track
    lastPoint += totalOffset;
    discretizedPoints.push_back(lastPoint);
}

}
