#pragma once

#include "MathUtils.h"
#include "DepthCamera.h"

#include <cassert>

#include <algorithm>
#include <memory>
#include <deque>
#include <map>
#include <vector>
#include <fstream>

namespace grl {

enum TrackState
{
    grlRecorderPointAdded, // Buffer was full and one of the points from it was added to the track
    grlRecorderPointBuffered, // Point buffered, but waiting for the buffer to be full
    grlRecorderPointSkipped, // Can happen, whenever the offset in the whole buffer was not big enought
};

enum TrackedHand
{
    grlTrackHandNone = 0x0,
    grlTrackHandLeft = 0x1,
    grlTrackHandRight = 0x2,
};

//////////////////////////////////////////
// TrackOffsets                         //
//////////////////////////////////////////

struct TrackOffsets
{
    Vec3f operator()(int i) const;
    void clear();
    float getLength() const;

    Vec3f startingPoint;
    std::vector<OrientedTransformation> transformations;
};

inline Vec3f
TrackOffsets::operator()(int i) const
{
    assert(i < transformations.size());

    Vec3f finalPoint = startingPoint;
    auto it = transformations.begin();
    for (int n = 0; n < i; ++i, ++it)
        finalPoint += it->getTransformationVector();

    return finalPoint;
}

inline float 
TrackOffsets::getLength() const
{
    float len = 0;
    for (auto it = transformations.cbegin(); it != transformations.end(); ++it)
        len += it->getDistance();
    return len;
}

inline void
TrackOffsets::clear()
{
    startingPoint = Vec3f(0.0f, 0.0f, 0.0f);
    transformations.clear();
}

//////////////////////////////////////////
// TrackPoints                          //
//////////////////////////////////////////

struct TrackPoints
{
    Vec3f & operator()(int i);
    const Vec3f & operator()(int i) const;
    const TrackPoints & operator+=(Vec3f translation);

    std::vector<Vec3f> points;
};

inline const TrackPoints &
TrackPoints::operator+=(Vec3f translation)
{
    for (auto it = points.begin(); it != points.cend(); ++it)
        *it += translation;
    return *this;
}

inline TrackPoints
operator+(const TrackPoints &track, Vec3f translation)
{
    TrackPoints result;
    result.points.reserve(track.points.size());

    for (auto it = track.points.begin(); it != track.points.cend(); ++it)
        result.points.push_back(*it + translation);

    return result;
}

inline Vec3f &
TrackPoints::operator()(int i)
{
    assert(i < points.size());
    return points[i];
}

inline const Vec3f &
TrackPoints::operator()(int i) const
{
    assert(i < points.size());
    return points[i];
}

inline void
offsetsToPoints(const TrackOffsets &offsets, TrackPoints &points, bool excludeStart = false)
{
    points.points.clear();
    Vec3f currentPoint = excludeStart ? Vec3f(0.0f, 0.0f, 0.0f) : offsets.startingPoint;

    points.points.push_back(currentPoint);
    for (auto it = offsets.transformations.cbegin(); it != offsets.transformations.end(); ++it) {
        currentPoint += it->getTransformationVector();
        points.points.push_back(currentPoint);
    }
}

//////////////////////////////////////////
// Gesture Descriptors                  //
//////////////////////////////////////////

struct OnlineGestureDescriptor
{
    bool tracked;
    Vec3f position;
    Vec3f translation;
    float distanceMoved;
};

// For tracks loaded from file
struct ProcessedOfflineGestureDescriptor
{
    const std::string *gestureName;
    std::unique_ptr<TrackPoints> track;
    TrackedHand hand;
};

// For tracks recorded live
struct NewOfflineGestureDescriptor
{
    const std::string *gestureName;
    const TrackOffsets *track;
    TrackedHand hand;
};

// Describing recognized track
struct OfflineGestureDescriptor
{
    const std::string *gestureName = nullptr;
    const TrackPoints *matchedTrack = nullptr;
    TrackPoints originalTrack;
    TrackedHand hand;
    float difference;

    OfflineGestureDescriptor & operator=(const OfflineGestureDescriptor source);
};

inline OfflineGestureDescriptor & 
OfflineGestureDescriptor::operator=(const OfflineGestureDescriptor source)
{
    gestureName = source.gestureName;
    matchedTrack = source.matchedTrack;
    originalTrack = source.originalTrack;
    hand = source.hand;
    difference = source.difference;
    return *this;
}

//////////////////////////////////////////
// TrackDatabase                        //
//////////////////////////////////////////

using TracksPtrsVector = std::vector<std::unique_ptr<TrackPoints> >;
using NamedTracksVectors = std::map<std::string, TracksPtrsVector>;

class TrackDatabase
{
public:
    bool addTrack(const std::string &name, std::unique_ptr<TrackPoints> track);
    void removeTracks(const std::string &name);
    void clear();

    void getNames(std::vector<const std::string *> &trackNames) const;
    const TracksPtrsVector * getTracks(const std::string &name) const;

    bool writeToFile(std::ofstream & file) const;

    NamedTracksVectors::const_iterator cbegin() const;
    NamedTracksVectors::const_iterator cend() const;

private:
    NamedTracksVectors _db;
};

inline bool
TrackDatabase::addTrack(const std::string &name, std::unique_ptr<TrackPoints> track)
{
    // Try to insert the element into the map
	TracksPtrsVector tracksVector;
    auto insertResult = _db.insert(std::make_pair(name, std::move(tracksVector)));

    // If the new trackVector was added or not, we still want to add track's pointer
    insertResult.first->second.push_back(std::move(track));

    return !insertResult.second;
}

inline void
TrackDatabase::getNames(std::vector<const std::string *> &trackNames) const
{
    trackNames.clear();
    for (auto it = _db.cbegin(); it != _db.cend(); ++it)
        trackNames.push_back(&(it->first));
}

inline void
TrackDatabase::clear()
{
    _db.clear();
}

inline void
TrackDatabase::removeTracks(const std::string &name)
{
    _db.erase(name);
}

inline const TracksPtrsVector *
TrackDatabase::getTracks(const std::string &name) const
{
    auto it = _db.find(name);
    if (it == _db.cend())
        return nullptr;
    else
        return &(it->second);
}

inline NamedTracksVectors::const_iterator
TrackDatabase::cbegin() const
{
    return _db.cbegin();
}

inline NamedTracksVectors::const_iterator 
TrackDatabase::cend() const
{
    return _db.cend();
}

//////////////////////////////////////////
// TrackRecorder                        //
//////////////////////////////////////////

class TrackRecorder
{
public:
    TrackRecorder(float minOffset, uint8_t step, size_t bufferLen, size_t maxTrackSize = 0);

    void clear();
    const TrackOffsets & getTrack() const;
    size_t getLength() const;

    TrackState addPoint(Vec3f point);

private:
    std::deque<Vec3f> _buffer;
    Vec3f _lastPoint;
    TrackOffsets _track;
    float _minOffset;
    size_t _bufferLen;
    size_t _maxTrackSize;
    int _currentFrame = 0;
    uint8_t _step;
    bool _recording = false;
};

inline
TrackRecorder::TrackRecorder(float minOffset, uint8_t step, size_t bufferLen, size_t maxTrackSize)
{
    _minOffset = minOffset;
    _step = step;
    _bufferLen = bufferLen;
    _maxTrackSize = maxTrackSize;
}

inline void
TrackRecorder::clear()
{
    _recording = false;
    _buffer.clear();
    _track.clear();
    _currentFrame = 0;
}

inline const TrackOffsets &
TrackRecorder::getTrack() const
{
    return _track;
}

inline size_t
TrackRecorder::getLength() const
{
    return _track.transformations.size();
}

//////////////////////////////////////////
// GestureTracker                       //
//////////////////////////////////////////

struct TrackerConfig
{
    // Set to true if the left hand right hand should be tracked separately for offline gesture recognition.
    // In that situation, if the track was recorded by right hand, it won't be recognized by the right hand.
    bool commonDatabase = true;
    float minDistance; // What is the minimum movement distance between two points
    int16_t framesIdleReset; // How many frames should be skipped if the hand is being idle before reset
    int16_t trackingLength; // How many measurements should be buffered
    int8_t frameSkip; // How many frames should be skipped before next point for offline tracking will be added
    int8_t filterPoints; // How many points should be buffered for median filter for track adding
};

class GestureTracker
{
public:
    using TransformationsIterator = std::iterator<std::forward_iterator_tag, OrientedTransformation>;

    virtual ~GestureTracker();

    virtual bool init(std::unique_ptr<TrackerConfig> config);
    virtual void destroy();
    virtual void update(const Joint &leftHand, const Joint &rightHand);

    virtual void processTrack(const TrackOffsets &offsets, TrackPoints &processed) = 0;

    const OfflineGestureDescriptor & getOfflineDescriptorLeft() const;
    const OfflineGestureDescriptor & getOfflineDescriptorRight() const;
    const OnlineGestureDescriptor & getOnlineDescriptorLeft() const;
    const OnlineGestureDescriptor & getOnlineDescriptorRight() const;

    const TrackDatabase & getDatabaseLeft() const;
    const TrackDatabase & getDatabaseRight() const;

    Vec3f getTrackOriginLeft() const;
    Vec3f getTrackOriginRight() const;

    void getRawTrackLeft(TrackPoints &track);
    void getRawTrackRight(TrackPoints &track);

    uint8_t getRecognizedTracks() const;

    bool addTrack(const NewOfflineGestureDescriptor &descriptor);
    bool addTrack(ProcessedOfflineGestureDescriptor &descriptor);

    bool writeDatabaseToFile(const std::string &fileName) const;

protected:
    struct TrackFeatures
    {
        OnlineGestureDescriptor onlineDescriptor;
        OfflineGestureDescriptor offlineDescriptor;
        Vec3f trackOrigin = Vec3f(0.0f, 0.0f, 0.0f);
        TrackPoints track;
        TrackDatabase database;
        int16_t currentFrame = 0;
        int16_t resetCount = 0;
        Vec3f trackEnd = Vec3f(0.0f, 0.0f, 0.0f);
        std::deque<Vec3f> filterBuffer;
        std::deque<OrientedTransformation> trackBuffer; 
        bool gatheringPoints = false;
    };

    std::unique_ptr<TrackerConfig> _config;

    TrackFeatures _leftFeatures;
    TrackFeatures _rightFeatures;
    uint8_t _recognitionStatus;

    bool recognizeTrack(const TrackPoints &track, TrackedHand hand);
    virtual float compareTracks(const TrackPoints &first, const TrackPoints &second) = 0;
    virtual void processTrack(const std::deque<OrientedTransformation> &offsets, TrackPoints &processed) = 0;

    static OnlineGestureDescriptor getOnlineForJoint(const OnlineGestureDescriptor &earlierDescriptor, const Joint &hand);

private:
    void updateBuffer(const Joint& hand);
    void convertDequeToTrack(TrackPoints &track, TrackedHand hand);
};

inline bool
GestureTracker::init(std::unique_ptr<TrackerConfig> config)
{
    if (config.get() == nullptr)
        return false;

    _config = std::move(config);
    _recognitionStatus = 0;

    return true;
}

inline
GestureTracker::~GestureTracker()
{
        destroy();

}

inline void
GestureTracker::destroy()
{
    _leftFeatures.database.clear();
    _rightFeatures.database.clear();
    _config.release();
}

inline void
GestureTracker::update(const Joint &leftHand, const Joint &rightHand)
{
    assert(leftHand.type == LEFT_HAND && rightHand.type == RIGHT_HAND);
    _leftFeatures.onlineDescriptor = getOnlineForJoint(_leftFeatures.onlineDescriptor, leftHand);
    _rightFeatures.onlineDescriptor = getOnlineForJoint(_rightFeatures.onlineDescriptor, rightHand);

    updateBuffer(leftHand);
    updateBuffer(rightHand);
}

inline OnlineGestureDescriptor
GestureTracker::getOnlineForJoint(const OnlineGestureDescriptor &earlierDescriptor, const Joint &hand)
{
    OnlineGestureDescriptor newDescriptor;

    newDescriptor.tracked = hand.tracked;
    newDescriptor.position = hand.coordWorld;
    newDescriptor.translation = newDescriptor.position - earlierDescriptor.position;
    newDescriptor.distanceMoved = newDescriptor.translation.length();

    return newDescriptor;
}

inline const OfflineGestureDescriptor & 
GestureTracker::getOfflineDescriptorLeft() const
{
    return _leftFeatures.offlineDescriptor;
}

inline const OfflineGestureDescriptor &
GestureTracker::getOfflineDescriptorRight() const
{
    return _rightFeatures.offlineDescriptor;
}

inline const OnlineGestureDescriptor &
GestureTracker::getOnlineDescriptorLeft() const
{
    return _leftFeatures.onlineDescriptor;
}

inline const OnlineGestureDescriptor &
GestureTracker::getOnlineDescriptorRight() const
{
    return _rightFeatures.onlineDescriptor;
}

inline void
GestureTracker::getRawTrackLeft(TrackPoints &track)
{
    convertDequeToTrack(track, grlTrackHandLeft);
}

inline void
GestureTracker::getRawTrackRight(TrackPoints &track)
{
    convertDequeToTrack(track, grlTrackHandRight);
}

inline Vec3f 
GestureTracker::getTrackOriginLeft() const
{
    return _leftFeatures.trackOrigin;
}

inline Vec3f 
GestureTracker::getTrackOriginRight() const
{
    return _rightFeatures.trackOrigin;
}

inline uint8_t 
GestureTracker::getRecognizedTracks() const
{
    return _recognitionStatus;
}

inline const TrackDatabase &
GestureTracker::getDatabaseLeft() const
{
    return _leftFeatures.database;
}

inline const TrackDatabase &
GestureTracker::getDatabaseRight() const
{
    return _config->commonDatabase ? _leftFeatures.database : _rightFeatures.database;
}

inline bool
GestureTracker::writeDatabaseToFile(const std::string &fileName) const
{
    std::ofstream ofs;
    ofs.open(fileName);
    if (!ofs.is_open())
        return false;

    if (_config->commonDatabase) {
        ofs << "// Common database\n";
        return _leftFeatures.database.writeToFile(ofs);
    }

    ofs << "// Left database\n";
    if(!_leftFeatures.database.writeToFile(ofs))
        return false;
    ofs << "\n// Right database\n";
    return _rightFeatures.database.writeToFile(ofs);
}

}
