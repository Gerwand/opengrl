#pragma once

#include <grl/utils/Catalogue.h>
#include <grl/classification/Features.h>

#include <map>

namespace grl {

/**
 * Object which can be used for KNN classification.
 * It allows the user to store the elements categorized by the name. It is adding
 * the ID of the class for each unique name.
 * Multiple features can be stored for each name. Together with feature, the
 * information about the object which features were recorded are also stored.
 *
 * @tparam _ObjectType Type of the object that is going to be assigned together
 *                     with features.
 * @tparam _FeatureType Type of the elements for the features vector.
 * @tparam _FeatureSize Length of the feature vector.
 */
template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
class KNNClassificator
{
public:
    // Shorter type for vector of the features.
    using Features = VectorOfFeatures<_FeatureType, _FeatureSize>;

    // Structure for assigning object and the features together.
    struct ObjectWithFeatures {
        _ObjectType object;
        Features features;
    };

    // Descriptor for KNN classification.
    struct KNNDescriptor {
        // Name of the class that was found as the right one
        const std::string *matchName;
        // Number of matches for the found class
        size_t matchNumber;
        // ID of the matched class
        size_t matchClassID;
        // Averate distance from the matched neighbours
        float matchAverageDistance;
    };

    /**
     * Add object together with feature to the classificator. The class name is
     * being used for categorizing and distinction. If the class name was already
     * used, the object is simply appended.
     *
     * @returns ID of the class for which the object was added.
     */
    size_t addObjectWithFeatures(const std::string &className,
                                 std::unique_ptr<ObjectWithFeatures> objectWithFeatures);

    /**
     * Remove the specified class from the KNN classificator.
     */
    void removeClass(const std::string &className);

    /**
     * Get all objects with features associated with the given class.
     */
    void getClass(const std::string &className,
                  std::vector<ObjectWithFeatures *> &objectWithFeaturesVector) const;

    /**
     * Classify the vector of the features.
     *
     * @param features Vector of features which must be classified.
     * @param numberOfNeighbours Max number of nearest neighbours to consider for
     *        classification.
     * @returns structure describing the match information
     */
    KNNDescriptor classify(const Features &features,
                           size_t numberOfNeighbours) const;
private:
    using KNNSet = std::multimap< float, size_t, std::greater<float> >;
    // For faster conversion from one type to the other, store both associations
    // - classes to names, and names to classes.
    std::map<std::string, size_t> _nameWithClass;
    std::map<size_t, std::string> _classWithName;
    Catalogue<size_t, ObjectWithFeatures> _classFeatures;

    // Counter for creating new classes
    size_t classCounter = 0;

    void getNearestNeighbours(const Features &features,
                              size_t numberOfNeighbours,
                              KNNSet &knn) const;
};

template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
size_t KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::addObjectWithFeatures(
    const std::string &className,
    std::unique_ptr<ObjectWithFeatures> objectWithFeatures)
{
    // Check if new class is needed
    size_t classID;

    auto nameWithID = _nameWithClass.find(className);
    if (nameWithID != _nameWithClass.end()) {
        // If there is a class existing, just read it's ID
        classID = nameWithID->second;
    } else {
        // Create new ID otherwise
        classID = ++classCounter;
        _nameWithClass[className] = classID;
        _classWithName[classID] = className;
    }

    _classFeatures.addElement(classID, std::move(objectWithFeatures));

    return classID;
}

template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
void KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::removeClass(
    const std::string &className)
{
    auto nameWithID = _nameWithClass.find(className);
    if (nameWithID == _nameWithClass.end())
        return;

    // Remove the class, but do not change the class ID counter for simplicity
    _classWithName.erase(nameWithID->second);
    _classFeatures.removeCategory(nameWithID->second);
    _nameWithClass.erase(nameWithID);
}

template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
void KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::getClass(
    const std::string &className,
    std::vector<KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::ObjectWithFeatures *> &objectWithFeaturesVector) const
{
    objectWithFeaturesVector.clear();
    size_t id = _nameWithClass.at(className);

    auto elements = _classFeatures.getElements(id);
    // It can be nullptr if there is no class called "class name"
    if (elements == nullptr)
        return;

    // Convert unqiue_ptrs to raw pointers
    for (auto it = elements->cbegin(); it != elements->cend(); ++it)
        objectWithFeaturesVector.push_back(it->get());
}

template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
typename KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::KNNDescriptor
KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::classify(
    const KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::Features &features,
    size_t numberOfNeighbours) const
{
    KNNSet knn;
    KNNDescriptor descriptor;

    // Find the N nearest negihbours
    getNearestNeighbours(features, numberOfNeighbours, knn);

    // Now, as the neighborus were found, fill out the descriptor...

    // First, calculate entries for all of the classes
    std::vector<size_t> classHistogram(classCounter);
    std::fill(classHistogram.begin(), classHistogram.end(), 0);

    for (auto it = knn.cbegin(); it != knn.cend(); ++it)
        // Classes are starting indexing from 1, so need to subtract 1
        ++classHistogram[it->second - 1];

    // Get the element with highest matches. In case of multiple ones, the one
    // with smaller class ID is being taken.
    std::vector<std::vector<size_t>::const_iterator> maxElements;
    std::vector<size_t>::const_iterator result = classHistogram.cbegin();
    while (true) {
        result = std::max_element(result, classHistogram.cend());
        if (result == classHistogram.cend())
            break;
        maxElements.push_back(result);
        ++result;
    };
 
    size_t matches;
    size_t matchedClass;
    if (maxElements.empty()) {
        matches = 0;
    } else if (maxElements.size() == 1) {
        matchedClass = std::distance(classHistogram.cbegin(), maxElements[0]) + 1;
        matches = *maxElements[0];
    } else {
        bool found = false;
        for (auto it = knn.crbegin(); it != knn.crend(); ++it) {
        // If there are more max elements, get the one with best distance...
            for (auto el = maxElements.cbegin(); el != maxElements.cend(); ++el) {
                matchedClass = std::distance(classHistogram.cbegin(), *el) + 1;
                if (it->second == matchedClass) {
                    matches = **el;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
    }

    // Now, fill out the descriptor
    descriptor.matchName = &(_classWithName.at(matchedClass));
    descriptor.matchNumber = matches;
    descriptor.matchClassID = matchedClass;

    // And the finally, get the average distance for the match
    float averageDistance = 0;
    for (auto it = knn.cbegin(); it != knn.cend(); ++it) {
        if (it->second == matchedClass)
            averageDistance += it->first;
    }
    descriptor.matchAverageDistance = averageDistance / matches;

    return descriptor;
}

template <typename _ObjectType, typename _FeatureType, size_t _FeatureSize>
void KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::getNearestNeighbours(
    const KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::Features &features,
    size_t numberOfNeighbours,
    KNNClassificator<_ObjectType, _FeatureType, _FeatureSize>::KNNSet &knn) const
{
    for (auto itClass = _classFeatures.cbegin(); itClass != _classFeatures.cend(); ++itClass) {
        size_t classID = itClass->first;

        // Find all nearest neighbours
        for (auto itDescriptor = itClass->second.cbegin();
             itDescriptor != itClass->second.cend();
             ++itDescriptor) {

            // It is a unique pointer, so we must get the raw pointer of it in
            // order to get the features (it should be in fact the shared pointer).
            auto otherFeatures  = itDescriptor->get()->features;
            float distance = features.getDistance(otherFeatures);

            // If there are alredy K nearest neighbours, it must be decided if
            // one must be released or not
            if (knn.size() == numberOfNeighbours) {
                // Check if the new distance is smaller than the largest one.
                if (distance < knn.cbegin()->first) {
                    // Make space for the new value
                    knn.erase(knn.cbegin());
                } else {
                    // Check next value
                    continue;
                }
            }

            // As the multimap is sorting the values by the key, is is handling
            // all the work for of us.
            knn.insert({distance, classID});
        }
    }
}

}
