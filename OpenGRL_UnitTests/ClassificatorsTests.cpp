#include "stdafx.h"
#include "CppUnitTest.h"

#include <grl/track/TrackClassificator.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenGRL_UnitTests_Classificators {

static bool operator==(const grl::Vec3f v1, const grl::Vec3f v2)
{
    return grl::absBetween(v1.x - v2.x, grl::epsilon) &&
        grl::absBetween(v1.y - v2.y, grl::epsilon) &&
        grl::absBetween(v1.z - v2.z, grl::epsilon);
}

TEST_CLASS(CatalogueTester)
{
private:
    grl::Catalogue<std::string, int> _catalogue;

public:
    CatalogueTester()
    {
        Logger::WriteMessage("--In CatalogueTester");
    }

    ~CatalogueTester()
    {
        Logger::WriteMessage("--CatalogueTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareCatalogue)
    {
        _catalogue.clear();

        // a
        _catalogue.addElement("a", std::move(std::make_unique<int>(1)));
        _catalogue.addElement("a", std::move(std::make_unique<int>(2)));
        _catalogue.addElement("a", std::move(std::make_unique<int>(3)));

        // b
        _catalogue.addElement("b", std::move(std::make_unique<int>(4)));
        _catalogue.addElement("b", std::move(std::make_unique<int>(5)));

        // c
        _catalogue.addElement("c", std::move(std::make_unique<int>(6)));
        _catalogue.addElement("c", std::move(std::make_unique<int>(7)));
        _catalogue.addElement("c", std::move(std::make_unique<int>(8)));
    }

    TEST_METHOD(getAllCategories)
    {
        Logger::WriteMessage("----In adding");

        std::vector<const std::string *> categories;
        _catalogue.getAllCategories(categories);

        Assert::AreEqual("a", categories[0]->data());
        Assert::AreEqual("b", categories[1]->data());
        Assert::AreEqual("c", categories[2]->data());

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(getElements)
    {
        Logger::WriteMessage("----In adding");

        auto b = _catalogue.getElements("b");
        Assert::AreEqual(4, *b->at(0));
        Assert::AreEqual(5, *b->at(1));

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(counters)
    {
        Logger::WriteMessage("----In adding");

        Assert::AreEqual(3ULL, _catalogue.getNumberOfCategories());
        Assert::AreEqual(8ULL, _catalogue.getNumberOfElements());

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(remove)
    {
        Logger::WriteMessage("----In adding");

        _catalogue.removeCategory("b");
        Assert::IsNull(_catalogue.getElements("b"));
        Assert::AreEqual(2ULL, _catalogue.getNumberOfCategories());
        Assert::AreEqual(6ULL, _catalogue.getNumberOfElements());

        std::vector<const std::string *> categories;
        _catalogue.getAllCategories(categories);
        Assert::AreEqual("a", categories[0]->data());
        Assert::AreEqual("c", categories[1]->data());

        Logger::WriteMessage("----adding Done");
    }

    TEST_METHOD(iterator)
    {
        Logger::WriteMessage("----In adding");

        std::vector<const char *> strings = { "a", "b", "c" };
        auto its = strings.cbegin();
        for (auto it = _catalogue.cbegin(); it != _catalogue.cend(); ++it, ++its) {
            Assert::AreEqual(*its, it->first.data());
        }

        Logger::WriteMessage("----adding Done");
    }
};


TEST_CLASS(FeaturesTester)
{
public:
    FeaturesTester()
    {
        Logger::WriteMessage("--In FeaturesTester");
    }

    ~FeaturesTester()
    {
        Logger::WriteMessage("--FeaturesTester Done");
    }

    TEST_METHOD(featureFloat)
    {
        Logger::WriteMessage("----In featureFloat");
        grl::Feature<float> _featuref1(-3.0f), _featuref2(0.0f);
        _featuref2.set(6.0f);

        Assert::AreEqual(81.0f, _featuref1.getSquareDistance(_featuref2), grl::epsilon);
        Assert::AreEqual(81.0f, _featuref2.getSquareDistance(_featuref1), grl::epsilon);

        Logger::WriteMessage("----featureFloat Done");
    }

    TEST_METHOD(featureVector)
    {
        Logger::WriteMessage("----In featureVector");
        grl::Feature<grl::Vec3f> _featurev1(grl::Vec3f(0.0f, 5.0f, 0.0f));
        grl::Feature<grl::Vec3f> _featurev2(grl::Vec3f(0.0f, 0.0f, 0.0f));
        _featurev2.set(grl::Vec3f(0.0f, 7.0f, 0.0f));

        Assert::AreEqual(4.0f, _featurev1.getSquareDistance(_featurev2), grl::epsilon);
        Assert::AreEqual(4.0f, _featurev2.getSquareDistance(_featurev1), grl::epsilon);

        Logger::WriteMessage("----featureVector Done");
    }

    TEST_METHOD(vectorFeaturesFloat)
    {
        Logger::WriteMessage("----In vectorFeaturesFloat");
        std::vector<float> vf1 = { 1.0f, 5.0f, 0.0f };
        std::array<grl::Feature<float>, 3> vf2 = {
            grl::Feature<float>(2.0f),
            grl::Feature<float>(7.0f),
            grl::Feature<float>(2.0f)
        };
        grl::VectorOfFeatures<float, 3> _vfeaturesf1(vf1);
        grl::VectorOfFeatures<float, 3> _vfeaturesf2(vf2);

        Assert::AreEqual(3.0f, _vfeaturesf1.getDistance(_vfeaturesf2), grl::epsilon);
        Assert::AreEqual(3.0f, _vfeaturesf2.getDistance(_vfeaturesf1), grl::epsilon);

        Logger::WriteMessage("----vectorFeaturesFloat Done");
    }

    TEST_METHOD(vectorFeaturesVector)
    {
        Logger::WriteMessage("----In vectorFeaturesVector");
        std::vector<grl::Feature<grl::Vec3f> > vv1 = {
            grl::Feature<grl::Vec3f>({ 1.0f, 0.0f, 0.0f }),
            grl::Feature<grl::Vec3f>({ 0.0f, 5.0f, 0.0f }),
            grl::Feature<grl::Vec3f>({ 0.0f, 0.0f, 0.0f })
        };
        std::array<grl::Vec3f, 3> vv2 = {
            grl::Vec3f{ 2.0f, 0.0f, 0.0f },
            grl::Vec3f{ 0.0f, 7.0f, 0.0f },
            grl::Vec3f{ 0.0f, 0.0f, 2.0f }
        };
        grl::VectorOfFeatures<grl::Vec3f, 3> _vfeaturesv1(vv1);
        grl::VectorOfFeatures<grl::Vec3f, 3> _vfeaturesv2(vv2);

        Assert::AreEqual(3.0f, _vfeaturesv1.getDistance(_vfeaturesv2), grl::epsilon);
        Assert::AreEqual(3.0f, _vfeaturesv2.getDistance(_vfeaturesv1), grl::epsilon);

        Logger::WriteMessage("----vectorFeaturesVector Done");
    }
};

TEST_CLASS(KNNClassificatorTester)
{
private:
    grl::KNNClassificator<void *, float, 1> _knn;

    using Features = grl::KNNClassificator<void *, float, 1>::Features;
    using Descriptor = grl::KNNClassificator<void *, float, 1>::ObjectWithFeatures;
public:
    KNNClassificatorTester()
    {
        Logger::WriteMessage("--In KNNClassificatorTester");
    }

    ~KNNClassificatorTester()
    {
        Logger::WriteMessage("--KNNClassificatorTester Done");
    }

    TEST_METHOD_INITIALIZE(prepareClasses)
    {
        // Around 1.0f
        Features features(std::vector<float>({ 1.0f }));
        _knn.addObjectWithFeatures("1", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 0.8f }));
        _knn.addObjectWithFeatures("1", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 1.1f }));
        _knn.addObjectWithFeatures("1", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 0.9f }));
        _knn.addObjectWithFeatures("1", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 1.2f }));
        _knn.addObjectWithFeatures("1", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));

        // Around 2.0f
        features = Features(std::vector<float>({ 2.0f }));
        _knn.addObjectWithFeatures("2", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 2.1f }));
        _knn.addObjectWithFeatures("2", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 2.2f }));
        _knn.addObjectWithFeatures("2", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 1.9f }));
        _knn.addObjectWithFeatures("2", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 1.8f }));
        _knn.addObjectWithFeatures("2", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));

        // Around 4.0f
        features = Features(std::vector<float>({ 3.9f }));
        _knn.addObjectWithFeatures("4", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 4.0f }));
        _knn.addObjectWithFeatures("4", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 3.8f }));
        _knn.addObjectWithFeatures("4", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 4.1f }));
        _knn.addObjectWithFeatures("4", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));

        // Around 6.0f
        features = Features(std::vector<float>({ 6.0f }));
        _knn.addObjectWithFeatures("6", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 5.7f }));
        _knn.addObjectWithFeatures("6", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
        features = Features(std::vector<float>({ 6.2f }));
        _knn.addObjectWithFeatures("6", std::move(
            std::make_unique<Descriptor>(
                Descriptor{ nullptr, features })));
    }

    TEST_METHOD(classify)
    {
        Logger::WriteMessage("----In classify");

        Features feat1(std::vector<float>({ 1.0f }));
        auto descriptor1 = _knn.classify(feat1, 5);
        Assert::AreEqual("1", descriptor1.matchName->data());
        Assert::AreEqual(5ULL, descriptor1.matchNumber);
        Assert::AreEqual(1ULL, descriptor1.matchClassID);
        Assert::AreEqual(0.12f, descriptor1.matchAverageDistance, grl::epsilon);

        Features feat2(std::vector<float>({ 6.0f }));
        auto descriptor2 = _knn.classify(feat2, 5);
        Assert::AreEqual("6", descriptor2.matchName->data());
        Assert::AreEqual(3ULL, descriptor2.matchNumber);
        Assert::AreEqual(4ULL, descriptor2.matchClassID);

        Features feat3(std::vector<float>({ 3.05f }));
        auto descriptor3 = _knn.classify(feat3, 5);
        Assert::AreEqual("4", descriptor3.matchName->data());
        Assert::AreEqual(3ULL, descriptor3.matchNumber);
        Assert::AreEqual(3ULL, descriptor3.matchClassID);

        Logger::WriteMessage("----classify Done");
    }
};

TEST_CLASS(DiscretizedTrackClassificationTester)
{
private:
    using Features = grl::DiscretizedTrackClassification<5>::Features;

    grl::DiscretizedTrackClassification<5> _discrete;

    // Line like this:
    // __
    std::vector<grl::Vec3f> _points1 = { {5.0f, 0.0f, 0.0f}, {10.0f, 0.0f, 0.0f} };
    std::vector<grl::Vec3f> _features1 = {
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
    };

    // Line like this:
    //  __
    // |____
    std::vector<grl::Vec3f> _points2 = {
        {3.0f, 3.0f, 0.0f},
        {2.0f, 3.0f, 0.0f},
        {2.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 2.0f, 0.0f},
        {4.0f, 1.0f, 0.0f},
    };
    std::vector<grl::Vec3f> _features2 = {
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
    };

    // Line like this:
    //  ____
    // |    |____
    std::vector<grl::Vec3f> _points3 = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.5f, 1.0f, 0.0f},
        {1.5f, 0.0f, 0.0f},
        {3.0f, 0.0f, 0.0f},
    };
    std::vector<grl::Vec3f> _features3 = {
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f/sqrtf(2.0f), -1.0f/sqrtf(2.0f), 0.0f},
        {1.0f/sqrtf(2.0f), -1.0f/sqrtf(2.0f), 0.0f},
        {1.0f, 0.0f, 0.0f},
    };

    std::vector< std::vector<grl::Vec3f> > _learnFor2 = {
        {
          { 0.0f, 0.0f, 0.0f },
          { 1.0f, 0.0f, 0.0f },
          { 1.0f, -1.0f, 0.0f },
          { 0.0f, -1.0f, 0.0f },
          { 0.0f, -2.0f, 0.0f },
          { 1.0f, -2.0f, 0.0f }
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { 1.1f, 0.1f, 0.0f },
          { 0.9f, -0.9f, 0.0f },
          { 0.0f, -1.0f, 0.0f },
          { 0.2f, -2.2f, 0.0f },
          { 1.1f, -1.9f, 0.0f }
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { 1.0f, -0.3f, 0.0f },
          { 1.0f, -1.0f, 0.0f },
          { 0.2f, -1.1f, 0.0f },
          { -0.2f, -2.0f, 0.0f },
          { 1.0f, -2.0f, 0.0f }
        },
    };

    std::vector<grl::Vec3f> _testFor2 = {
        { 0.0f, 0.0f, 0.0f },
        { 1.2f, 0.1f, 0.0f },
        { 0.7f, -1.1f, 0.0f },
        { 0.1f, -0.9f, 0.0f },
        { 0.2f, -2.2f, 0.0f },
        { 1.1f, -1.9f, 0.0f },
    };

    std::vector< std::vector<grl::Vec3f> > _learnFor5 = {
        {
          { 1.0f, 0.0f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
          { 0.0f, -1.0f, 0.0f },
          { 1.0f, -1.0f, 0.0f },
          { 1.0f, -2.0f, 0.0f },
          { 0.0f, -2.0f, 0.0f },
        },

        {
          { 1.1f, 0.1f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
          { 0.0f, -1.0f, 0.0f },
          { 0.9f, -0.9f, 0.0f },
          { 1.1f, -1.9f, 0.0f },
          { 0.2f, -2.2f, 0.0f },
        },

        {
          { 1.0f, -0.3f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
          { 0.2f, -1.1f, 0.0f },
          { 1.0f, -1.0f, 0.0f },
          { 1.0f, -2.0f, 0.0f },
          { -0.2f, -2.0f, 0.0f },
        },
    };

    std::vector<grl::Vec3f> _testFor5 = {
        { 1.2f, 0.1f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.1f, -0.9f, 0.0f },
        { 0.7f, -1.1f, 0.0f },
        { 1.1f, -1.9f, 0.0f },
        { 0.2f, -2.2f, 0.0f },
    };

    std::vector< std::vector<grl::Vec3f> > _learnForRight = {
        {
          { 0.0f, 0.0f, 0.0f },
          { 1.0f, 0.0f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { 1.0f, 0.1f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { 0.9f, -0.2f, 0.0f },
        },
    };

    std::vector<grl::Vec3f> _testForRight = {
        { 0.0f, 0.0f, 0.0f },
        { 1.1f, 0.2f, 0.0f },
    };

    std::vector< std::vector<grl::Vec3f> > _learnForLeft = {
        {
          { 1.0f, 0.0f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
        },

        {
          { 1.0f, 0.1f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
        },

        {
          { 0.9f, -0.2f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
        },
    };

    std::vector<grl::Vec3f> _testForLeft = {
        { 1.1f, 0.2f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    };

    std::vector< std::vector<grl::Vec3f> > _learnForUp = {
        {
          { 0.0f, 0.0f, 0.0f },
          { 0.0f, 1.0f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { -0.1f, 1.2f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { -0.2f, 0.9f, 0.0f },
        },
    };

    std::vector<grl::Vec3f> _testForUp = {
        { 0.0f, 0.0f, 0.0f },
        { 0.2f, 0.8f, 0.0f },
    };

    std::vector< std::vector<grl::Vec3f> > _learnForC = {
        {
          { 0.0f, 0.0f, 0.0f },
          { -1.0f, 0.0f, 0.0f },
          { -1.0f, -1.0f, 0.0f },
          { 0.0f, -1.0f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { -1.0f, 0.2f, 0.0f },
          { -1.2f, -1.0f, 0.0f },
          { 0.0f, -0.8f, 0.0f },
        },

        {
          { 0.0f, 0.0f, 0.0f },
          { -1.1f, 0.2f, 0.0f },
          { -1.2f, -1.0f, 0.0f },
          { 0.0f, -1.2f, 0.0f },
        },
    };

    std::vector<grl::Vec3f> _testForC = {
        { 0.0f, 0.0f, 0.0f },
        { -1.1f, -0.2f, 0.0f },
        { -0.9f, -1.1f, 0.0f },
        { 0.1f, -0.9f, 0.0f },
    };

public:
    DiscretizedTrackClassificationTester()
    {
        Logger::WriteMessage("--In DiscretizedTrackClassificationTester");
    }

    ~DiscretizedTrackClassificationTester()
    {
        Logger::WriteMessage("--DiscretizedTrackClassificationTester Done");
    }

    TEST_METHOD(extractFeatures)
    {
        Logger::WriteMessage("----In extractFeatures");

        grl::TrackPoints points;
        Features features;

        for (auto it = _points1.begin(); it != _points1.end(); ++it)
            points.addPoint(*it);
        _discrete.extractFeatures(points, features);
        auto itf = _features1.cbegin();
        for (auto it = features.cbegin(); it != features.cend(); ++it, ++itf)
            Assert::IsTrue(*itf == it->get());
        points.clear();

        for (auto it = _points2.begin(); it != _points2.end(); ++it)
            points.addPoint(*it);
        _discrete.extractFeatures(points, features);
        itf = _features2.cbegin();
        for (auto it = features.cbegin(); it != features.cend(); ++it, ++itf)
            Assert::IsTrue(*itf == it->get());
        points.clear();

        for (auto it = _points3.begin(); it != _points3.end(); ++it)
            points.addPoint(*it);
        _discrete.extractFeatures(points, features);
        itf = _features3.cbegin();
        for (auto it = features.cbegin(); it != features.cend(); ++it, ++itf)
            Assert::IsTrue(*itf == it->get());

        Logger::WriteMessage("----extractFeatures Done");
    }


    TEST_METHOD(classify)
    {
        Logger::WriteMessage("----In classify");

        _discrete.init(3);

        // Learn 2
        for (auto it = _learnFor2.begin(); it != _learnFor2.end(); ++it) {
            grl::TrackPoints learnFor2;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnFor2.addPoint(*itp);
            }

            _discrete.updateDatabase("2", learnFor2);
        }

        // Learn 5
        for (auto it = _learnFor5.begin(); it != _learnFor5.end(); ++it) {
            grl::TrackPoints learnFor5;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnFor5.addPoint(*itp);
            }

            _discrete.updateDatabase("5", learnFor5);
        }

        // Learn c
        for (auto it = _learnForC.begin(); it != _learnForC.end(); ++it) {
            grl::TrackPoints learnForC;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnForC.addPoint(*itp);
            }

            _discrete.updateDatabase("c", learnForC);
        }

        // Learn up
        for (auto it = _learnForUp.begin(); it != _learnForUp.end(); ++it) {
            grl::TrackPoints learnForUp;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnForUp.addPoint(*itp);
            }

            _discrete.updateDatabase("up", learnForUp);
        }

        // Learn right
        for (auto it = _learnForRight.begin(); it != _learnForRight.end(); ++it) {
            grl::TrackPoints learnForRight;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnForRight.addPoint(*itp);
            }

            _discrete.updateDatabase("right", learnForRight);
        }

        // Learn left
        for (auto it = _learnForLeft.begin(); it != _learnForLeft.end(); ++it) {
            grl::TrackPoints learnForLeft;
            for (auto itp = it->begin(); itp != it->end(); ++itp) {
                learnForLeft.addPoint(*itp);
            }

            _discrete.updateDatabase("left", learnForLeft);
        }

        // Test 2
        grl::TrackPoints testFor2;
        for (auto it = _testFor2.begin(); it != _testFor2.end(); ++it)
            testFor2.addPoint(*it);

        auto desc = _discrete.recognize(testFor2);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("2", desc.trackCategory->data());

        // Test 5
        grl::TrackPoints testFor5;
        for (auto it = _testFor5.begin(); it != _testFor5.end(); ++it)
            testFor5.addPoint(*it);

        desc = _discrete.recognize(testFor5);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("5", desc.trackCategory->data());

        // Test C
        grl::TrackPoints testForC;
        for (auto it = _testForC.begin(); it != _testForC.end(); ++it)
            testForC.addPoint(*it);

        desc = _discrete.recognize(testForC);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("c", desc.trackCategory->data());

        // Test up
        grl::TrackPoints testForUp;
        for (auto it = _testForUp.begin(); it != _testForUp.end(); ++it)
            testForUp.addPoint(*it);

        desc = _discrete.recognize(testForUp);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("up", desc.trackCategory->data());

        // Test right
        grl::TrackPoints testForRight;
        for (auto it = _testForRight.begin(); it != _testForRight.end(); ++it)
            testForRight.addPoint(*it);

        desc = _discrete.recognize(testForRight);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("right", desc.trackCategory->data());

        // Test left
        grl::TrackPoints testForLeft;
        for (auto it = _testForLeft.begin(); it != _testForLeft.end(); ++it)
            testForLeft.addPoint(*it);

        desc = _discrete.recognize(testForLeft);

        Assert::IsNotNull(desc.trackCategory);
        Assert::AreEqual("left", desc.trackCategory->data());

        Logger::WriteMessage("----classify Done");
    }
};

}
