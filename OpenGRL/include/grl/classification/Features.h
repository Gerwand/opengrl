#pragma once

#include <grl/utils/MathUtils.h>

#include <array>
#include <vector>

namespace grl {

//////////////////////////////////////////
// Feature                              //
//////////////////////////////////////////

/**
 * Template for describing feature of the object which can be used for
 * classification. It is simply encapsulating type T and providing interface for
 * calculating distance and getters and setters.
 *
 * @tparam T type of the encapsulated feature.
 */
template <typename T>
class Feature
{
public:
    /**
     * Empty constructor
     */
    Feature();
    /**
     * Create feature using the object of type T, which it is encapsulating.
     */
    Feature(const T &feature);

    /**
     * Get distance from another feature using euclidesan distance but without
     * the square root. It can be used by the KNN to avoid executing square root
     * for each feature in the feature vector, which can be costly.
     * The default template implementation is calculating
     * (this->get() - other->get())*(this->get() - other->get()), and it must
     * return float, so it the type T needs to define other behavior, the
     * template specialization of this method should be created for type T.
     *
     * @param other The other feature from which the square distance must be
     *              calculated.
     *
     * @returns square distance from the another feature
     */
    float getSquareDistance(const Feature &other) const;

    /**
     * Get the reference to the encapsulated object T representing the feature.
     */
    const T & get() const;

    /**
     * Set the new feature.
     */
    void set(const T &feature);

private:
    T _feature;
};

// Typedefs for convenience
typedef Feature<Vec3f> FeatureVector;
typedef Feature<float> FeatureFloat;
typedef Feature<int> FeatureInt;
typedef Feature<uint8_t> FeatureByte;

template <typename T>
Feature<T>::Feature()
{

}

template <typename T>
Feature<T>::Feature(const T &feature)
{
    _feature = feature;
}

template <typename T>
float Feature<T>::getSquareDistance(const Feature<T> &other) const
{
    T diff = _feature - other.get();
    return diff * diff;
}

template <typename T>
const T & Feature<T>::get() const
{
    return _feature;
}

template <typename T>
void Feature<T>::set(const T &feature)
{
    _feature = feature;
}

template <>
inline float Feature<Vec3f>::getSquareDistance(const Feature<Vec3f> &other) const
{
    Vec3f difference = _feature - other.get();
    return difference.x * difference.x +
           difference.y * difference.y +
           difference.z * difference.z;
}


//////////////////////////////////////////
// VectorOfFeatures                     //
//////////////////////////////////////////

/**
 * The class which is representing vector of features with the constant length.
 * All features must be of the same type.
 *
 * @tparam T type of the features, in fact it will be Feature<T>
 * @tparam size number of features inside the vector - it will be constant
 */
template <typename T, size_t size>
class VectorOfFeatures
{
public:
    using ConstIterator = typename std::array<Feature<T>, size>::const_iterator;
    /**
    * Empty constructor.
    */
    VectorOfFeatures();
    /**
    * Create the vector of features using std::vector of the type T. Objects
    * inside will be converted to Feature<T> and assigned to this object.
    * The size of the vector must be the same as the 'size' template parameter.
    */
    VectorOfFeatures(const std::vector<T> &features);
    /**
    * Create the vector of features using std::vector of the type Feature<T>.
    * The size of the vector must be the same as the 'size' template parameter.
    */
    VectorOfFeatures(const std::vector< Feature<T> > &features);
    /**
    * Create the vector of features using std::array of the type T and the same
    * size as the VectorOfFeatures. Objects inside will be converted to
    * Feature<T> and assigned to this object.
    */
    VectorOfFeatures(const std::array<T, size> &features);
    /**
    * Create the vector of features using std::array of the features and the
    * same size as the VectorOfFeatures.
    */
    VectorOfFeatures(const std::array<Feature<T>, size> &features);

    /**
     * Get distance from the another vector of features of the same type and the
     * same length.
     * It is the euclidean distance which is sqrt(sum(this[i] - other[i])^2)
     */
    float getDistance(VectorOfFeatures &other) const;

    ConstIterator cbegin() const;
    ConstIterator cend() const;

private:
    std::array<Feature<T>, size> _features;
};

template <typename T, size_t size>
VectorOfFeatures<T, size>::VectorOfFeatures()
{

}

template <typename T, size_t size>
VectorOfFeatures<T, size>::VectorOfFeatures(const std::vector<T> &features)
{
    assert(features.size() == size && "Source vector smaller than features vector!");
    auto source = features.cbegin();
    auto destination = _features.begin();

    while (source != features.cend()) {
        *destination = Feature<T>(*source);

        ++source;
        ++destination;
    }
}

template <typename T, size_t size>
VectorOfFeatures<T, size>::VectorOfFeatures(const std::vector< Feature<T> > &features)
{
    assert(features.size() == size && "Source vector smaller than features vector!");
    auto source = features.cbegin();
    auto destination = _features.begin();

    while (source != features.cend()) {
        *destination = *source;

        ++source;
        ++destination;
    }
}

template <typename T, size_t size>
VectorOfFeatures<T, size>::VectorOfFeatures(const std::array<T, size> &features)
{
    auto source = features.cbegin();
    auto destination = _features.begin();

    while (source != features.cend()) {
        *destination = Feature<T>(*source);

        ++source;
        ++destination;
    }
}

template <typename T, size_t size>
VectorOfFeatures<T, size>::VectorOfFeatures(const std::array<Feature<T>, size> &features)
{
    auto source = features.cbegin();
    auto destination = _features.begin();

    while (source != features.cend()) {
        *destination = *source;

        ++source;
        ++destination;
    }
}

template <typename T, size_t size>
float VectorOfFeatures<T, size>::getDistance(VectorOfFeatures<T, size> &other) const
{
    float totalDistance = 0;

    auto thisFeature = _features.cbegin();
    auto otherFeature = other._features.cbegin();
    while (thisFeature != _features.cend()) {

        totalDistance += thisFeature->getSquareDistance(*otherFeature);

        ++thisFeature;
        ++otherFeature;
    }

    return sqrt(totalDistance);
}

template <typename T, size_t size>
typename VectorOfFeatures<T, size>::ConstIterator
VectorOfFeatures<T, size>::cbegin() const
{
    return _features.cbegin();
}

template <typename T, size_t size>
typename VectorOfFeatures<T, size>::ConstIterator
VectorOfFeatures<T, size>::cend() const
{
    return _features.cend();
}
}
