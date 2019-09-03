#pragma once

#include <grl/track/TrackPoints.h>
#include <grl/track/TrackRecorderQueue.h>

#include <grl/utils/MathUtils.h>
#include <grl/camera/DepthCamera.h>

#include <cassert>

#include <algorithm>
#include <memory>
#include <map>
#include <vector>
#include <fstream>

namespace grl {

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

//////////////////////////////////////////
// GestureTracker                       //
//////////////////////////////////////////

/**
 * Configuration that can be passed to the GestureTracker. It is defining the
 * track and the database in which all the tracks will be kept.
 */
struct TrackerConfig
{
    float minDistance; // What is the minimum movement distance between two points
    int16_t framesIdleReset; // How many frames should be skipped if the hand is being idle before reset
    int16_t trackingLength; // How many measurements should be buffered
    int8_t frameSkip; // How many frames should be skipped before next point for offline tracking will be added
    int8_t filterPoints; // How many points should be buffered for median filter for track adding
};

/**
 * GestureTracker can be used to record the gestures from the hand movement.
 *
 * If the user needs to keep separate tracks for left and right hands, the two
 * GestureTrackers can be spawned.
 */
class GestureTracker
{
public:
    /**
     * Indicate what happened with in this frame with the point received from the
     * median filter.
     * - grlTrackerAdded - this frame, the filtered point was added to a track.
     * - grlTrackerBuffered - this frame, the input point was buffered to the filter, but not added to a track..
     * - grlTrackerSkipped - the filtered point was skipped due to too small offset size.
     * - grlTrackerReset - the track was reset due to too long inactivity.
     */
    enum UpdateState {
        grlTrackerAdded,
        grlTrackerBuffered,
        grlTrackerSkipped,
        grlTrackerReset,
    };

    /**
     * Initialize the tracker with the given config.
     */
    void init(const TrackerConfig &config);

    /**
     * When the new frame from depth camera arrives, the joint representing hand
     * should be passed to this function. Depending on the action taken inside,
     * the approriate UpdateState will be returned.
     */
    UpdateState update(const Joint &hand);

    /**
     * Get the recorded track and save it in the object provided by the user.
     */
    void getCapturedTrack(TrackPoints &points);

    /**
     * Get online descriptor for the track.
     */
    OnlineGestureDescriptor getOnlineDescriptor() const;

private:
    TrackerConfig _config;
    TrackRecorderQueue _recorder;
    OnlineGestureDescriptor _lastOnline;
    size_t _idleFramesCount;

    OnlineGestureDescriptor calculateOnlineGesture(const Joint &hand);
};

}
