#pragma once

#include <grl/classification/Classificator.h>
#include <grl/track/TrackTools.h>

#include <string>

namespace grl {

class TrackClassificator
{
public:
    struct TrackMatchDescriptor
    {
        const std::string *trackCategory;
        float score1;
        size_t score2;
    };

    virtual TrackMatchDescriptor recognize(const TrackPoints &track) const = 0;
};

template <size_t _Segments>
class DiscretizedTrackClassification : public TrackClassificator
{
    using KNN = KNNClassificator<TrackPoints, Vec3f, _Segments>;
public:
    using Features = VectorOfFeatures<Vec3f, _Segments>;
    using TrackWithFeatures = typename KNN::ObjectWithFeatures;

    void init(size_t knn);

    TrackMatchDescriptor recognize(const TrackPoints &track) const override;

    void updateDatabase(const std::string &name, const TrackPoints &track);
    void removeTrack(const std::string &name);

    void extractFeatures(const TrackPoints &track, Features &features) const;
private:
    static constexpr float SegmentLength = 1.0f / _Segments;

    KNN _classificator;
    size_t _knn;
};

template <size_t _s>
void DiscretizedTrackClassification<_s>::init(size_t knn)
{
    _knn = knn;
}

template <size_t _s>
typename DiscretizedTrackClassification<_s>::TrackMatchDescriptor
DiscretizedTrackClassification<_s>::recognize(const TrackPoints &track) const
{
    TrackMatchDescriptor descriptor;

    Features features;
    extractFeatures(track, features);
    typename KNN::KNNDescriptor knnDescriptor = _classificator.classify(features, _knn);

    descriptor.trackCategory = knnDescriptor.matchName;
    descriptor.score1 = knnDescriptor.matchAverageDistance;
    descriptor.score2 = knnDescriptor.matchNumber;


    return descriptor;
}

template <size_t _s>
void DiscretizedTrackClassification<_s>::updateDatabase(
    const std::string &name,
    const TrackPoints &track)
{
    auto descriptor = std::make_unique<KNN::ObjectWithFeatures>();
    descriptor->object = track;
    extractFeatures(track, descriptor->features);

    _classificator.addObjectWithFeatures(name, std::move(descriptor));
}

template <size_t _s>
void DiscretizedTrackClassification<_s>::removeTrack(const std::string &name)
{
    _classificator.removeClass(name);
}

template <size_t _s>
void DiscretizedTrackClassification<_s>::extractFeatures(
    const TrackPoints &track,
    Features &features) const
{
    TrackOffsets offsets;
    TrackTools::trackPointsToTrackOffsets(track, offsets);
    // Set the track beginning to 0, as it's not important
    offsets.setStartingPoint(Vec3f(0.0f, 0.0f, 0.0f));

    TrackOffsets discretizedOffsets;
    discretizedOffsets.addPoint(Vec3f(0.0f, 0.0f, 0.0f));

    Vec3f accumulatedOffsets = Vec3f(0.0f, 0.0f, 0.0f);
    float accumulatedSegmentsLength = 0.0f;
    // Delta for discretization
    const float delta = offsets.getTrackLength() / _s;
    for (auto it = offsets.begin(); it != offsets.end(); ++it) {
        float segmentLength = it->getDistance();

        // If it is next point in a sequence, subtract part of the delta from the previous offsets
        float deltaLeft = delta - accumulatedSegmentsLength;
        if (segmentLength < deltaLeft) {
            // If the segment is smaller than the rest of the delta, append current
            // segment to the next offset and go for the other point.
            accumulatedOffsets += it->getTransformationVector();
            accumulatedSegmentsLength += segmentLength;
            continue;
        }

        // Now, the deltaLeft length offset toward the next point in the original
        // track must be added. deltaLeft probably will be smaller than it->getDistance().
        Vec3f lastPoint = discretizedOffsets.getLastPoint();
        accumulatedOffsets += it->getUnitVector() * deltaLeft;
        lastPoint += accumulatedOffsets;
        discretizedOffsets.addPoint(lastPoint);

        // Now, the rest of the segment length must be processed.
        segmentLength -= deltaLeft;

        // It is possible that the remaining length in the segment can be
        // discretized to few whole segments (for example, if the segment will
        // be long, straight line). First, calculate number of the whole segments.
        int wholeSegments = static_cast<int>(segmentLength/delta);
        // Then, we know that they are going to have size length, so they can
        // be processed in the loop using the same vector.
        Vec3f deltaOffset = it->getUnitVector() *  delta;
        for (int i = 0; i < wholeSegments; ++i) {
            lastPoint += deltaOffset;
            discretizedOffsets.addPoint(lastPoint);
        }

        // As the whole segments were processed, they length of the segment must
        // be reduced in size.
        segmentLength -= wholeSegments*delta;

        // Now, add the remaining segment length for the processing.
        accumulatedOffsets = segmentLength * it->getUnitVector();
        accumulatedSegmentsLength = segmentLength;
    }

    // Sometimed, due to floating point error, last point can be skipped. In that situation,
    // simply add the reamining offset.
    if ((discretizedOffsets.getPointsCount() - 1) < _s)
        discretizedOffsets.addPoint(accumulatedOffsets);

    // Finally, extract the features
    std::array<Vec3f, _s> featuresVector;
    auto itf = featuresVector.begin();
    for (auto it = discretizedOffsets.begin(); it != discretizedOffsets.end(); ++it, ++itf) {
        // The transformation vector in relation to the last point is considered
        // as a feature.
        *itf = it->getUnitVector();
    }

    // Now, assign the features to the user
    features = Features(featuresVector);
}

}
