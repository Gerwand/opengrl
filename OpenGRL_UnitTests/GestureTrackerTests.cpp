#include "stdafx.h"
#include "CppUnitTest.h"

#include <grl/track/GestureTracker.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenGRL_UnitTests_GestureTracker {

static const struct
{
    std::array<grl::Vec3f, 5> points = { grl::Vec3f(2.0f, 1.0f, 0.0f),
                                        grl::Vec3f(3.0f, 1.0f, 0.0f),
                                        grl::Vec3f(3.0f, 2.0f, 0.0f),
                                        grl::Vec3f(2.0f, 2.0f, 0.0f),
                                        grl::Vec3f(2.0f, 3.0f, 0.0f) };
    std::array<grl::Vec3f, 4> vectors = { grl::Vec3f(1.0f, 0.0f, 0.0f),
                                        grl::Vec3f(0.0f, 1.0f, 0.0f),
                                        grl::Vec3f(-1.0f, 0.0f, 0.0f),
                                        grl::Vec3f(0.0f, 1.0f, 0.0f) };
    std::array<float, 4> vectorsLengths = { 1.0f,
                                            1.0f,
                                            1.0f,
                                            1.0f };

    float len = 4.0f;
} SamplePoints;

static bool operator==(const grl::Vec3f v1, const grl::Vec3f v2)
{
    return grl::absBetween(v1.x - v2.x, grl::epsilon) &&
           grl::absBetween(v1.y - v2.y, grl::epsilon) &&
           grl::absBetween(v1.z - v2.z, grl::epsilon);
}

TEST_CLASS(TrackPointsTester)
{
private:
    grl::TrackPoints _track;

public:
    TrackPointsTester()
    {
        Logger::WriteMessage("--In TrackPointsTester");
    }

    ~TrackPointsTester()
    {
        Logger::WriteMessage("--TrackPointsTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareTrack)
    {
        _track.clear();
        for (auto it = SamplePoints.points.cbegin(); it != SamplePoints.points.cend(); ++it)
            _track.addPoint(*it);
    }

    TEST_METHOD(get)
    {
        Logger::WriteMessage("----In get");

        size_t i = 0;
        for (auto it = SamplePoints.points.cbegin(); it != SamplePoints.points.cend(); ++it, ++i) {
            Assert::IsTrue(*it == _track.getPoint(i));
            Assert::IsTrue(*it == _track(i));
            Assert::IsTrue(*it == _track.getElement(i));
            Assert::IsTrue(*it == _track[i]);
        }


        Assert::IsTrue(SamplePoints.points.back() == _track.getLastPoint());

        Logger::WriteMessage("----get Done");
    }

    TEST_METHOD(lengths)
    {
        Logger::WriteMessage("----In lengths");

        Assert::AreEqual(_track.getTrackLength(), SamplePoints.len);
        Assert::AreEqual(_track.getPointsCount(), SamplePoints.points.size());

        Logger::WriteMessage("----lengths Done");
    }

    TEST_METHOD(iterator)
    {
        Logger::WriteMessage("----In iterator");

        size_t i = 0;
        for (auto it = _track.begin(); it != _track.end(); ++it, ++i)
            Assert::IsTrue(*it == _track.getElement(i));

        Assert::AreEqual(i, _track.getPointsCount());

        Logger::WriteMessage("----get iterator");
    }

};

TEST_CLASS(TrackOffsetsTester)
{
private:
    grl::TrackOffsets _track;

public:
    TrackOffsetsTester()
    {
        Logger::WriteMessage("--In TrackOffsetsTester");
    }

    ~TrackOffsetsTester()
    {
        Logger::WriteMessage("--TrackOffsetsTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareTrack)
    {
        _track.clear();
        for (auto it = SamplePoints.points.cbegin(); it != SamplePoints.points.cend(); ++it)
            _track.addPoint(*it);
    }

    TEST_METHOD(get)
    {
        Logger::WriteMessage("----In get");

        size_t i = 0;
        for (auto it = SamplePoints.points.cbegin(); it != SamplePoints.points.cend(); ++it, ++i) {
            Assert::IsTrue(*it == _track.getPoint(i));
            Assert::IsTrue(*it == _track(i));
        }

        i = 0;
        for (auto it = SamplePoints.vectors.cbegin(); it != SamplePoints.vectors.cend(); ++it, ++i) {
            Assert::IsTrue(*it == _track.getElement(i).getUnitVector());
            Assert::IsTrue(*it == _track[i].getUnitVector());
        }

        Assert::IsTrue(SamplePoints.points.back() == _track.getLastPoint());

        Logger::WriteMessage("----get Done");
    }

    TEST_METHOD(lengths)
    {
        Logger::WriteMessage("----In lengths");

        Assert::AreEqual(_track.getTrackLength(), SamplePoints.len);
        Assert::AreEqual(_track.getPointsCount(), SamplePoints.points.size());

        size_t i = 0;
        for (auto it = SamplePoints.vectorsLengths.cbegin(); it != SamplePoints.vectorsLengths.cend(); ++it, ++i)
            Assert::IsTrue(*it == _track[i].getDistance());

        Logger::WriteMessage("----lengths Done");
    }

    TEST_METHOD(iterator)
    {
        Logger::WriteMessage("----In iterator");

        size_t i = 0;
        for (auto it = _track.begin(); it != _track.end(); ++it, ++i)
            Assert::IsTrue(it->getTransformationVector() == _track.getElement(i).getTransformationVector());

        Assert::AreEqual(_track.getPointsCount() - 1, i);

        Logger::WriteMessage("----get iterator");
    }

    TEST_METHOD(startingPoint)
    {
        Logger::WriteMessage("----In startingPoint");

        grl::TrackOffsets track;
        grl::Vec3f samplePoint1(1.0f, 1.0f, 0.0f);
        grl::Vec3f samplePoint2(1.0f, 2.0f, 0.0f);
        grl::Vec3f samplePoint3(1.0f, 2.0f, 1.0f);

        track.setStartingPoint(samplePoint1);
        Assert::AreEqual(1ULL, track.getPointsCount());
        Assert::IsTrue(track.getLastPoint() == samplePoint1);

        track.addPoint(samplePoint2);
        Assert::AreEqual(2ULL, track.getPointsCount());
        Assert::IsTrue(track.getLastPoint() == samplePoint2);

        Assert::IsTrue(track(0) == samplePoint1);
        Assert::IsTrue(track(1) == samplePoint2);

        track.clear();
        Assert::AreEqual(0ULL, track.getPointsCount());
        track.addPoint(samplePoint3);
        Assert::IsTrue(track.getLastPoint() == samplePoint3);

        Assert::AreEqual(1ULL, track.getPointsCount());
        Assert::IsTrue(track(0) == samplePoint3);

        Logger::WriteMessage("----get startingPoint");
    }

};

TEST_CLASS(TrackRecorderTester)
{
private:
    std::vector<grl::Vec3f> _inputPoints;
    static constexpr size_t _numPoints = 60;

    struct {
        float offset = 1.5f;
        uint8_t step = 4;
        size_t bufferLen;
        size_t maxSize = 10;
    } _noMedianCfg, _medianCfg;

public:
    TrackRecorderTester()
    {
        Logger::WriteMessage("--In TrackRecorderTester");
        for (size_t i = 0; i < _numPoints; ++i)
            _inputPoints.push_back(grl::Vec3f((float)i, (float)(i + 1), (float)(i + 2)));
        _noMedianCfg.bufferLen = 1;
        _medianCfg.bufferLen = 3;
        _medianCfg.step = 2;
        _medianCfg.offset = 0.0f;
    }

    ~TrackRecorderTester()
    {
        Logger::WriteMessage("--TrackRecorderTester Done");
    }

    TEST_METHOD(adding)
    {
        Logger::WriteMessage("----In adding");

        grl::TrackRecorder recorder(_noMedianCfg.offset,
                                    _noMedianCfg.step,
                                    _noMedianCfg.bufferLen,
                                    _noMedianCfg.maxSize);

        // Check if every 5 is being added and rest is being buffered.
        for (size_t i = 0; i < _numPoints; ++i) {
            grl::TrackRecorder::RecorderState state;

            state = recorder.addPoint(_inputPoints[i]);
            if (i >= _noMedianCfg.maxSize * (_noMedianCfg.step+1)) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointSkipped);
            } else if (i == 0 || (i+1) % (_noMedianCfg.step+1)) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointBuffered);
            } else {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointAdded);
            }
        }

        recorder.getTrack().getPointsCount();

        Assert::AreEqual(recorder.getTrack().getPointsCount(), _noMedianCfg.maxSize);

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(skipSmallOffset)
    {
        Logger::WriteMessage("----In skipSmallOffset");

        grl::TrackRecorder recorder(_noMedianCfg.offset,
                                    _noMedianCfg.step,
                                    _noMedianCfg.bufferLen,
                                    _noMedianCfg.maxSize);

        // Check if every 5 is being added and rest is being buffered.
        for (size_t i = 0; i < 100; ++i) {
            grl::TrackRecorder::RecorderState state;

            state = recorder.addPoint(grl::Vec3f(1.0f, 1.0f, 1.0f));
            if (i == 0 || (i+1) % (_noMedianCfg.step+1)) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointBuffered);
            } else if (i == 4) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointAdded);
            } else {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointSkipped);
            }
        }

        Assert::AreEqual(recorder.getTrack().getPointsCount(), 1ULL);

        Logger::WriteMessage("----skipSmallOffset Done");
    }


    TEST_METHOD(median)
    {
        Logger::WriteMessage("----In median");

        grl::TrackRecorder recorder(_medianCfg.offset,
                                    _medianCfg.step,
                                    _medianCfg.bufferLen,
                                    _medianCfg.maxSize);

        grl::Vec3f point1(1.0f, 0.0f, 0.0f);
        grl::Vec3f point2(5.0f, 0.0f, 0.0f);
        grl::Vec3f point3(-1.0f, 0.0f, 0.0f);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointAdded);
        Assert::IsTrue(recorder.getTrack().getLastPoint() == point1);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        Assert::IsTrue(recorder.getTrack().getLastPoint() == point1);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        Assert::IsTrue(recorder.getTrack().getLastPoint() == point3);

        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        Assert::IsTrue(recorder.getTrack().getLastPoint() == point2);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        Assert::IsTrue(recorder.getTrack().getLastPoint() == point1);

        Logger::WriteMessage("----median Done");
    }

    TEST_METHOD(trackValidation)
    {
        Logger::WriteMessage("----In trackValidation");

        grl::TrackRecorder recorder(_noMedianCfg.offset,
                                    _noMedianCfg.step,
                                    _noMedianCfg.bufferLen,
                                    _noMedianCfg.maxSize);

        for (size_t i = 0; i < _numPoints; ++i)
            recorder.addPoint(_inputPoints[i]);

        for (size_t i = 0; i < recorder.getTrack().getPointsCount(); ++i) {
            size_t index = (i+1) * (_noMedianCfg.step+1) - 1;
            Assert::IsTrue(recorder.getTrack().getPoint(i) == _inputPoints[index]);
        }

        Logger::WriteMessage("----trackValidation Done");
    }

};

TEST_CLASS(TrackRecorderQueueTester)
{
private:
    std::vector<grl::Vec3f> _inputPoints;
    static constexpr size_t _numPoints = 60;

    struct
    {
        float offset = 1.5f;
        uint8_t step = 4;
        size_t bufferLen;
        size_t maxSize = 10;
    } _noMedianCfg, _medianCfg;

public:
    TrackRecorderQueueTester()
    {
        Logger::WriteMessage("--In TrackRecorderQueueTester");
        for (size_t i = 0; i < _numPoints; ++i)
            _inputPoints.push_back(grl::Vec3f((float)i, (float)(i + 1), (float)(i + 2)));
        _noMedianCfg.bufferLen = 1;
        _medianCfg.bufferLen = 3;
        _medianCfg.step = 2;
        _medianCfg.offset = 0.0f;
    }

    ~TrackRecorderQueueTester()
    {
        Logger::WriteMessage("--TrackRecorderQueueTester Done");
    }

    TEST_METHOD(adding)
    {
        Logger::WriteMessage("----In adding");

        grl::TrackRecorderQueue recorder(_noMedianCfg.offset,
                                         _noMedianCfg.step,
                                         _noMedianCfg.bufferLen,
                                         _noMedianCfg.maxSize);

        // Check if every 5 is being added and rest is being buffered.
        for (size_t i = 0; i < _numPoints; ++i) {
            grl::TrackRecorder::RecorderState state;

            state = recorder.addPoint(_inputPoints[i]);
            if (i == 0 || (i+1) % (_noMedianCfg.step+1)) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointBuffered);
            } else {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointAdded);
            }
        }


        grl::TrackPoints points;
        recorder.getTrack(points);

        Assert::AreEqual(points.getPointsCount(), _noMedianCfg.maxSize);

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(skipSmallOffset)
    {
        Logger::WriteMessage("----In skipSmallOffset");

        grl::TrackRecorderQueue recorder(_noMedianCfg.offset,
                                         _noMedianCfg.step,
                                         _noMedianCfg.bufferLen,
                                         _noMedianCfg.maxSize);

        // Check if every 5 is being added and rest is being buffered.
        for (size_t i = 0; i < 100; ++i) {
            grl::TrackRecorder::RecorderState state;

            state = recorder.addPoint(grl::Vec3f(1.0f, 1.0f, 1.0f));
            if (i == 0 || (i+1) % (_noMedianCfg.step+1)) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointBuffered);
            } else if (i == 4) {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointAdded);
            } else {
                Assert::IsTrue(state == grl::TrackRecorder::grlRecorderPointSkipped);
            }
        }

        grl::TrackPoints points;
        recorder.getTrack(points);

        Assert::AreEqual(points.getPointsCount(), 1ULL);

        Logger::WriteMessage("----skipSmallOffset Done");
    }


    TEST_METHOD(median)
    {
        Logger::WriteMessage("----In median");

        grl::TrackRecorderQueue recorder(_medianCfg.offset,
                                    _medianCfg.step,
                                    _medianCfg.bufferLen,
                                    _medianCfg.maxSize);

        grl::Vec3f point1(1.0f, 0.0f, 0.0f);
        grl::Vec3f point2(5.0f, 0.0f, 0.0f);
        grl::Vec3f point3(-1.0f, 0.0f, 0.0f);

        grl::TrackPoints points;

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointAdded);

        recorder.getTrack(points);
        Assert::IsTrue(points.getLastPoint() == point1);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        recorder.getTrack(points);
        Assert::IsTrue(points.getLastPoint() == point1);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        recorder.getTrack(points);
        Assert::IsTrue(points.getLastPoint() == point3);

        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        recorder.getTrack(points);
        Assert::IsTrue(points.getLastPoint() == point2);

        Assert::IsTrue(recorder.addPoint(point1) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point2) == grl::TrackRecorder::grlRecorderPointBuffered);
        Assert::IsTrue(recorder.addPoint(point3) == grl::TrackRecorder::grlRecorderPointAdded);
        recorder.getTrack(points);
        Assert::IsTrue(points.getLastPoint() == point1);

        Logger::WriteMessage("----median Done");
    }

    TEST_METHOD(trackValidation)
    {
        Logger::WriteMessage("----In trackValidation");

        grl::TrackRecorderQueue recorder(_noMedianCfg.offset,
                                    _noMedianCfg.step,
                                    _noMedianCfg.bufferLen,
                                    5); // Save only 5 last points

        for (size_t i = 0; i < _numPoints; ++i)
            recorder.addPoint(_inputPoints[i]);

        grl::TrackPoints points;
        size_t startIndex = (_numPoints/(_noMedianCfg.step+1) - 5)  * (_noMedianCfg.step+1) - 1;
        recorder.getTrack(points);
        for (size_t i = 0; i < points.getPointsCount(); ++i) {
            size_t index = startIndex + (i+1) * (_noMedianCfg.step+1);
            Assert::IsTrue(points.getPoint(i) == _inputPoints[index]);
        }

        Logger::WriteMessage("----trackValidation Done");
    }
};


TEST_CLASS(GestureTrackerTester)
{
private:
    std::vector<grl::Joint> _inputPoints;
    grl::Joint _inactive;
    static constexpr size_t _numPoints = 20;

    grl::TrackerConfig _config;
public:
    GestureTrackerTester()
    {
        Logger::WriteMessage("--In GestureTrackerTester");
        for (size_t i = 0; i < _numPoints; ++i) {
            grl::Joint joint;
            joint.coordWorld = grl::Vec3f((float)i, (float)(i + 1), (float)(i + 2));
            joint.tracked = true;
            _inputPoints.push_back(joint);
        }

        _inactive.coordWorld = grl::Vec3f(0.0f, 1.0f, -5.0f);
        _inactive.tracked = false;

        _config.filterPoints = 1;
        _config.framesIdleReset = 2;
        _config.frameSkip = 1;
        _config.minDistance = 1.0f;
        _config.trackingLength = 5;
    }

    ~GestureTrackerTester()
    {
        Logger::WriteMessage("--GestureTrackerTester Done");
    }

    TEST_METHOD(adding)
    {
        Logger::WriteMessage("----In adding");

        grl::GestureTracker tracker;
        tracker.init(_config);

        // Check if every 5 is being added and rest is being buffered.
        for (size_t i = 0; i < _numPoints; ++i) {
            grl::GestureTracker::UpdateState state;

            state = tracker.update(_inputPoints[i]);
            if (i == 0 || (i+1) % (_config.frameSkip+1)) {
                Assert::IsTrue(state == grl::GestureTracker::grlTrackerBuffered);
            } else {
                Assert::IsTrue(state == grl::GestureTracker::grlTrackerAdded);
            }
        }

        for (size_t i = 0; i < 5; ++i)
            Assert::IsTrue(tracker.update(_inactive) == grl::GestureTracker::grlTrackerSkipped);

        grl::TrackPoints points;
        tracker.getCurrentTrack(points);

        Assert::AreEqual(points.getPointsCount(), (size_t)_config.trackingLength);

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(idleReset)
    {
        Logger::WriteMessage("----In idleReset");

        grl::GestureTracker tracker;
        tracker.init(_config);
        for (size_t i = 0; i < _numPoints; ++i)
            tracker.update(_inputPoints[i]);

        grl::TrackPoints points;
        tracker.getCurrentTrack(points);

        grl::Joint lastJoint;
        lastJoint.coordWorld = points.getLastPoint();
        lastJoint.tracked = true;

        Assert::IsTrue(tracker.update(lastJoint) == grl::GestureTracker::grlTrackerBuffered);
        Assert::IsTrue(tracker.update(lastJoint) == grl::GestureTracker::grlTrackerSkipped);
        Assert::IsTrue(tracker.update(lastJoint) == grl::GestureTracker::grlTrackerBuffered);
        Assert::IsTrue(tracker.update(lastJoint) == grl::GestureTracker::grlTrackerReset);

        tracker.getCurrentTrack(points);

        Assert::AreEqual(0ULL, points.getPointsCount());

        Logger::WriteMessage("----idleReset Done");
    }

    TEST_METHOD(trackValidation)
    {
        Logger::WriteMessage("----In trackValidation");

        grl::GestureTracker tracker;
        tracker.init(_config);

        for (size_t i = 0; i < _numPoints; ++i)
            tracker.update(_inputPoints[i]);

        grl::TrackPoints points;
        tracker.getCurrentTrack(points);

        size_t startIndex = (_numPoints/(_config.frameSkip+1) - 5)  * (_config.frameSkip+1) - 1;
        for (size_t i = 0; i < points.getPointsCount(); ++i) {
            size_t index = startIndex + (i+1) * (_config.frameSkip+1);
            Assert::IsTrue(points.getPoint(i) == _inputPoints[index].coordWorld);
        }

        Logger::WriteMessage("----trackValidation Done");
    }
};

}
