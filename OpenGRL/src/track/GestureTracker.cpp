#include <grl/track/GestureTracker.h>

namespace grl {

//////////////////////////////////////////
// TrackRecorder                        //
//////////////////////////////////////////

using IndexedFloat = std::pair<size_t, float>;

struct {
        bool operator()(IndexedFloat a, IndexedFloat b) const {
            return a.second < b.second;
        }
} indexedFloatLess;

bool
TrackDatabase::writeToFile(std::ofstream & file) const
{
    // The file must be opened before wirting
    if (!file.is_open())
        return false;

    for (auto itdb = _db.cbegin(); itdb != _db.cend(); ++itdb) {
        // Replace ' ' with '_' for better names compatibility
        std::string nameString = itdb->first;
        std::replace(nameString.begin(), nameString.end(), ' ', '_');
        file << '\n' << itdb->first << " = {\n";
        for (auto itv = itdb->second.cbegin(); itv < itdb->second.cend(); ++itv) {
            file << "    {\n";
            const std::vector<Vec3f> &points = (*itv)->points;
            for (auto itp = points.cbegin(); itp != points.cend(); ++itp)
                file << "        {" << itp->x << "f, " << itp->y << "f, " << itp->z << "f},\n";
            file << "    },\n";
        }
        file << "};\n";
    }

    file << "\nnames = {\n";
    for (auto itdb = _db.cbegin(); itdb != _db.cend(); ++itdb) {
        // Replace ' ' with '_' for better names compatibility
        std::string nameString = itdb->first;
        std::replace(nameString.begin(), nameString.end(), ' ', '_');
        file << "    " << nameString << ",\n";
    }
    file << "};\n";

    return true;
}

TrackState
TrackRecorder::addPoint(Vec3f point)
{
    // Check if the track is not full
    if (_track.transformations.size() > _maxTrackSize)
        return grlRecorderPointSkipped;

    // If already buffered max points, remove first one (oldest one) to make the place for the newer point
    if (_buffer.size() == _bufferLen)
        _buffer.pop_front();
    // Add point to the buffer - it will be used for median filter
    _buffer.push_back(point);

    // The point is added to track every step frames/points
    if (_currentFrame++ != _step)
        return grlRecorderPointBuffered;

    // Reset the frame counter
    _currentFrame = 0;

    // If buffer is full, get the point using median filter to reduce the noise
    std::vector<IndexedFloat> bufferDistances(_buffer.size());
    size_t index = 0;
    for (auto itBuffer = _buffer.cbegin(); itBuffer != _buffer.cend(); ++itBuffer, ++index) {
        IndexedFloat bufDistance;
        bufDistance.first = index;
        bufDistance.second = Vec3f::distance(*itBuffer, _lastPoint);
        bufferDistances.push_back(bufDistance);
    }
    std::sort(bufferDistances.begin(), bufferDistances.end(), indexedFloatLess);
    int midIndex = bufferDistances.size()/2;

    // If the moved distance was too small, do not add it to the track
    const IndexedFloat &midDistance = bufferDistances[midIndex];
    if (midDistance.second < _minOffset)
        return grlRecorderPointSkipped;

    // Add the point to the track
    const Vec3f &midPoint = _buffer[midDistance.first];
    // Check if it is the first point that should be recorded
    if (!_recording) {
        // Make this point a starting point and return
        _track.startingPoint = midPoint;
        _recording = true;
    } else {
        Vec3f offset = midPoint - _lastPoint;
        _track.transformations.push_back(offset);
    }
    _lastPoint = midPoint;
    
    return grlRecorderPointAdded;
}

bool
GestureTracker::recognizeTrack(const TrackPoints &track, TrackedHand hand)
{
    // If the hands shouldn't be distincted, use left's hand database as the common one
    TrackFeatures &features = hand == grlTrackHandRight ? _rightFeatures : _leftFeatures;
    const TrackDatabase &database = _config->commonDatabase ? _leftFeatures.database : features.database;

    // Iterate over database and calculate score for each track
    features.offlineDescriptor = OfflineGestureDescriptor{nullptr, nullptr, track, hand, std::numeric_limits<float>::infinity()};

    for (auto itGesture = database.cbegin(); itGesture != database.cend(); ++itGesture) {
        std::vector<float> scores;
        scores.reserve(itGesture->second.size());

        // Get score for each track in the gesture
        const TrackPoints *bestTrack;
        float minScore = std::numeric_limits<float>::infinity();
        for (auto itTracks = itGesture->second.cbegin(); itTracks != itGesture->second.cend(); ++itTracks) {
            float score = compareTracks(track, **itTracks);
            scores.push_back(score);
            // The lower the score, the better the track fits
            if (score < minScore) {
                minScore = score;
                bestTrack = itTracks->get();
            }
        }

        // Calculate the average score
        float avgScore = 0.0f;
        int validValues = 0;
        for (auto itScore = scores.cbegin(); itScore != scores.cend(); ++itScore) {
            // Do not count in the inifinite values
            if (*itScore == std::numeric_limits<float>::infinity())
                continue;

            avgScore += *itScore;
            ++validValues;
        }

        // Analyze the results
        if (validValues == 0)
            continue;
        
        avgScore /= validValues;
        if (avgScore < features.offlineDescriptor.difference) {
            features.offlineDescriptor.difference = avgScore;
            features.offlineDescriptor.matchedTrack = bestTrack;
            features.offlineDescriptor.gestureName = &itGesture->first;
        }
    }

    return features.offlineDescriptor.difference != std::numeric_limits<float>::infinity();
}

bool
GestureTracker::addTrack(const NewOfflineGestureDescriptor &descriptor)
{
    // If the hands shouldn't be distincted, use left's hand database as the common one
    TrackDatabase &destDatabase = (!_config->commonDatabase && descriptor.hand == grlTrackHandRight) ?
        _rightFeatures.database :
        _leftFeatures.database;

    auto upTrackPoints = std::make_unique<TrackPoints>();
    processTrack(*descriptor.track, *upTrackPoints);

    return destDatabase.addTrack(*descriptor.gestureName, std::move(upTrackPoints));
}

bool 
GestureTracker::addTrack(ProcessedOfflineGestureDescriptor & descriptor)
{
    TrackDatabase &destDatabase = (!_config->commonDatabase && descriptor.hand == grlTrackHandRight) ?
        _rightFeatures.database :
        _leftFeatures.database;

    return destDatabase.addTrack(*descriptor.gestureName, std::move(descriptor.track));
}

void
GestureTracker::updateBuffer(const Joint& hand)
{
    TrackedHand handType = hand.type == LEFT_HAND ? grlTrackHandLeft : grlTrackHandRight;
    TrackFeatures &features = handType == grlTrackHandLeft ? _leftFeatures : _rightFeatures;    

    if (!hand.tracked) {
        ++features.resetCount;
        return;
    }

    // Add the point to the buffer
    if (features.filterBuffer.size() == _config->filterPoints)
        features.filterBuffer.pop_front();
    features.filterBuffer.push_back(hand.coordWorld);

    // Analyze the buffer every frameSkip frames
    if (features.currentFrame++ != _config->frameSkip)
        return;

    // Reset the frame counter
    features.currentFrame = 0;

    // Get the relative movement since last saved frame
    std::vector<Vec3f> offsets(features.filterBuffer.size());
    std::copy(features.filterBuffer.cbegin(), features.filterBuffer.cend(), offsets.begin());
    for (auto it = offsets.begin(); it != offsets.end(); ++it)
        *it -= features.trackEnd;

    std::deque<std::pair<int, OrientedTransformation> > sortedFilterBuffer(offsets.size());
    auto itVec = offsets.cbegin();
    auto itOr = sortedFilterBuffer.begin();
    for (int i = 0; itVec != offsets.cend(); ++itVec, ++itOr) {
        itOr->first = i++;
        itOr->second.set(*itVec);
    }

    std::sort(sortedFilterBuffer.begin(), sortedFilterBuffer.end(),
              [](std::pair<int, OrientedTransformation> &a, std::pair<int, OrientedTransformation> &b) {
        return a.second.getDistance() < b.second.getDistance();
    });

    size_t midIndex = sortedFilterBuffer.size()/2;
    OrientedTransformation &medianTransformation = sortedFilterBuffer[midIndex].second;

    if (!features.gatheringPoints) {
        // Clear the recognition flag
        _recognitionStatus &= ~handType;
        int pointMedianIndex = sortedFilterBuffer[midIndex].first;
        features.trackEnd = features.trackOrigin = features.filterBuffer[pointMedianIndex];
        features.gatheringPoints = true;
        return;
    }

    // Check if the point moved at least minimum distance
    if (medianTransformation.getDistance() < _config->minDistance) {
        features.resetCount += _config->frameSkip;
        // If the hand was idle for too long, reset all buffers
        if (features.resetCount >= _config->framesIdleReset) {
            // Process the track - implementation defined
            if (features.trackBuffer.size() > 1) {
                processTrack(features.trackBuffer, features.track);
                // Now do some recognition
                if (recognizeTrack(features.track, handType))
                    _recognitionStatus |= handType;
            }

            features.resetCount = 0;
            features.filterBuffer.clear();
            features.trackBuffer.clear();
            features.trackEnd = Vec3f(0.0f, 0.0f, 0.0f);
            features.track.points.clear();
            features.trackOrigin = Vec3f(0.0f, 0.0f, 0.0f);
            features.gatheringPoints = false;
        }
        return;
    }

    // Track the track's end
    Vec3f tmp = medianTransformation.getTransformationVector();
    features.trackEnd += tmp;

    // If we came here, we must clean up the buffer
    if (features.trackBuffer.size() == _config->trackingLength) {
        OrientedTransformation firstTransformation = features.trackBuffer.front();
        features.trackBuffer.pop_front();
        features.trackOrigin += firstTransformation.getTransformationVector();
    }
    // For first point, set track's origin

    // Add the transformation at the end of the buffer
    features.trackBuffer.push_back(medianTransformation);
}

void
GestureTracker::convertDequeToTrack(TrackPoints &track, TrackedHand hand)
{
    TrackFeatures &features = hand == grlTrackHandLeft ? _leftFeatures : _rightFeatures;

    track.points.clear();
    // Reserve space for each offset + 1 for track origin
    track.points.reserve(features.trackBuffer.size() + 1);
    track.points.push_back(features.trackOrigin);

    for (auto it = features.trackBuffer.cbegin(); it != features.trackBuffer.cend(); ++it)
        track.points.push_back(it->getTransformationVector() + track.points.back());
}

}
