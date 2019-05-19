#include <grl/track/DiscretizedGestureTracker.h>

namespace grl {

bool
DiscretizedGestureTracker::init(std::unique_ptr<TrackerConfig> config)
{
    if (!GestureTracker::init(std::move(config)))
        return false;

    _discretizedConfig = static_cast<DiscretizedTrackerConfig *>(_config.get());

    return true;
}

float
DiscretizedGestureTracker::compareTracks(const TrackPoints &first, const TrackPoints &second)
{
    int firstSize = static_cast<int>(first.points.size());
    int secondSize = static_cast<int>(second.points.size());
    int sizeDiff = _discretizedConfig->maxCompareSegmentsDiff;

    if (!absBetween(firstSize - secondSize, sizeDiff))
	    return std::numeric_limits<float>::infinity();
    
    float score = 0;
    size_t comparedPoints;
    std::vector<Vec3f>::const_iterator itFirst;
    std::vector<Vec3f>::const_iterator itFirstEnd;
    std::vector<Vec3f>::const_iterator itSecond;
    if (firstSize <= secondSize) {
        comparedPoints = firstSize;
        itFirst = first.points.cbegin();
        itFirstEnd = first.points.cend();
        itSecond = second.points.cbegin();
    } else {
        comparedPoints = secondSize;
        itFirst = second.points.cbegin();
        itFirstEnd = second.points.cend();
        itSecond = first.points.cbegin();
    }
    
    for (; itFirst != itFirstEnd; ++itFirst, ++itSecond)
        score += Vec3f(*itFirst - *itSecond).length();
    
    // Calculate average score but ad penatly for each missing different point
    float averageScore = (comparedPoints + sizeDiff)*score/comparedPoints/comparedPoints;

    return averageScore;
}

}
