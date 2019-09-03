#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace grl {

/**
 * Catalogue, which is intended to assign multiple values to the single category.
 * Data is stored in the map, which is using the type K for catalogizing objects
 * of type T.
 *
 * @tparam K type for category value of the catalogue. All elements will be catalogued
 *           using this type.
 * @tparam T For each category vector of objects of type T will be assigned.
 */
template <typename K, typename T>
class Catalogue
{
public:
    // Unique pointer for type of T. It must be passed to the catalogue this way.
    using ElementPointer = std::unique_ptr<T>;
    // Vector of unique pointers for type T. Each
    using ElementsVector = std::vector<ElementPointer>;

private:
    using ElementsMap = std::map<K, ElementsVector>;

public:
    /**
     * Iterator for moving over all elements inside the catalogue.
     * The it->first is the object K which is categorizing elements, and the
     * it->second is the vector which is holding unique_ptrs for all of the
     * elements. The it->second->get() should be used to access the stored
     * elements directly.
     */
    using ConstIterator = typename ElementsMap::const_iterator;

    /**
     * Add element for the category determined by the category value. If the category
     * wasn't exising, it is created and the element is appended.
     *
     * @param category Category of the element.
     * @param element Element which is going to be added.
     * @return false if the category wasn't in the catalogue and had to be created,
     *         true if the category was already inside and the catalogue and new element
     *         was appended.
     */
    bool addElement(const K &category, ElementPointer element);

    /**
     * Get all elements of the given category.
     *
     * @return pointer to the vector containing unique pointers for all of the
     * elements.
     */
    const ElementsVector * getElements(const K &category) const;

    /**
     * Remove all elements of the given category.
     *
     * @param category Category of elements which must be removed.
     */
    void removeCategory(const K &category);

    /**
     * Clear all categories and elements inside the catalogue.
     */
    void clear();

    /**
     * Get all of the categories holded by the catalogue. They will be saved
     * in the categories input parameter.
     */
    void getAllCategories(std::vector<const K *> &categories) const;

    /**
     * Get number of all elements across all categories.
     */
    size_t getNumberOfElements() const;

    /**
     * Get number of all categories in the catalogue.
     */
    size_t getNumberOfCategories() const;

    /**
     * Get const forward iterator of the catalogue starting at first element.
     * The it->first element is of type K, and the it->second is of ElementsVector.
     */
    ConstIterator cbegin() const;
    /**
     * Get const forward iterator of the catalogue showing the end of the catalogue.
     */
    ConstIterator cend() const;

private:
    ElementsMap _map;
    size_t _numberOfElements = 0;
};

template <typename T>
using NamesCatalogue = Catalogue<std::string, T>;

template <typename K, typename T>
bool Catalogue<K, T>::addElement(const K &category, Catalogue<K, T>::ElementPointer element)
{
	ElementsVector elementsVector;

    // Try to insert the new element into the map, if it was not existing
    auto insertResult = _map.insert(std::make_pair(category, std::move(elementsVector)));

    insertResult.first->second.push_back(std::move(element));
    ++_numberOfElements;

    // True if the new result was in fact created, or false if only the existing one was returned.
    // So this method returns: true if the object for the category was existing, false if it had to be created.
    return !insertResult.second;
}

template <typename K, typename T>
void Catalogue<K, T>::removeCategory(const K &category)
{
    _numberOfElements -= _map[category].size();
    _map.erase(category);
}

template <typename K, typename T>
void Catalogue<K, T>::clear()
{
    _map.clear();
    _numberOfElements = 0;
}

template <typename K, typename T>
void Catalogue<K, T>::getAllCategories(std::vector<const K *> &categories) const
{
    categories.clear();
    for (auto it = _map.cbegin(); it != _map.cend(); ++it)
        categories.push_back(&(it->first));
}

template <typename K, typename T>
typename Catalogue<K, T>::ConstIterator Catalogue<K, T>::cbegin() const
{
    return _map.cbegin();
}

template <typename K, typename T>
typename Catalogue<K, T>::ConstIterator Catalogue<K, T>::cend() const
{
    return _map.cend();
}

template <typename K, typename T>
typename const Catalogue<K, T>::ElementsVector * Catalogue<K, T>::getElements(const K &category) const
{
    auto it = _map.find(category);
    if (it == _map.cend())
        return nullptr;
    else
        return &(it->second);
}

template <typename K, typename T>
size_t Catalogue<K, T>::getNumberOfElements() const
{
    return _numberOfElements;
}

template <typename K, typename T>
size_t Catalogue<K, T>::getNumberOfCategories() const
{
    return _map.size();
}

}
