#include <grl/track/GestureTracker.h>

namespace grl {

void GestureTracker::init(const TrackerConfig &config)
{
    _config = config;
    _recorder = TrackRecorderQueue(config.minDistance,
                                   config.frameSkip,
                                   config.filterPoints,
                                   config.trackingLength);
    memset(&_lastOnline , 0, sizeof(_lastOnline));
    _idleFramesCount = 0;
}

GestureTracker::UpdateState GestureTracker::update(const Joint &hand)
{
    calculateOnlineGesture(hand);

    // If the gesture is not tracked, the counting of the frames is freezed, as
    // the data is not reliable there.
    if (!hand.tracked)
        return grlTrackerSkipped;

    TrackRecorder::RecorderState updateState = _recorder.addPoint(hand.coordWorld);
    if (updateState == TrackRecorder::grlRecorderPointSkipped) {
        if (++_idleFramesCount == _config.framesIdleReset) {
            // Reset the track if it was idle for too long
            _recorder.clear();
            _idleFramesCount = 0;
            return grlTrackerReset;
        }
        return grlTrackerSkipped;
    } else if (updateState == TrackRecorder::grlRecorderPointAdded) {
        return grlTrackerAdded;
    }

    return grlTrackerBuffered;
}

void GestureTracker::getCapturedTrack(TrackPoints &points)
{
    _recorder.getTrack(points);
}

OnlineGestureDescriptor GestureTracker::getOnlineDescriptor() const
{
    return _lastOnline;
}

OnlineGestureDescriptor GestureTracker::calculateOnlineGesture(const Joint &hand)
{
    if (_lastOnline.tracked) {
        // Set translation to 0 if the previous frame wasn't tracked
        _lastOnline.translation = Vec3f(0.0f, 0.0f, 0.0f);
        _lastOnline.distanceMoved = 0.0f;
    } else {
        _lastOnline.translation = hand.coordWorld - _lastOnline.position;
        _lastOnline.distanceMoved = _lastOnline.translation.length();
    }

    // Common part
    _lastOnline.position = hand.coordWorld;
    _lastOnline.tracked = hand.tracked;

    return _lastOnline;
}

}
