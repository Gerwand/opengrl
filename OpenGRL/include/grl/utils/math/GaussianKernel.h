#pragma once

#include "Vectors.h"

#include <vector>
#include <cassert>

namespace grl {

/**
 * Class for generating and accessing gaussian kernel (matrix). It can be used
 * for filtering 2D data (or 1D).
 */
class GaussianKernel
{
public:
    /**
     * Initialize gaussian kernel with given parameters. It is always 2D square with
     * the same width and height.
     *
     * @param bandwidth The steepness of the kernel. The higher this value is, the
     * kernel will be smoothing more.
     * @param size Size of the kernel. Kernel is always a square.
     */
    GaussianKernel(float bandwidth, size_t size);

    /**
     * Get kernel's value at the given coordinates.
     *
     * @param x x coordinate.
     * @param y y coordinate.
     * @returns value in the kernel.
     */
    float & operator()(int x, int y);
    /**
     * Get kernel's value at the given coordinates.
     *
     * @param x x coordinate.
     * @param y y coordinate.
     * @returns value in the kernel.
     */
    const float & operator()(int x, int y) const;

     /**
     * Get size of the kernel
     *
     * @returns size of the square 2D kernel.
     */
    size_t getSize() const;

private:
    std::vector<float> _mask;
    size_t _n;
};

inline float & GaussianKernel::operator()(int x, int y)
{
    return _mask[x + y * _n];
}

inline const float & GaussianKernel::operator()(int x, int y) const
{
    return _mask[x + y * _n];
}


inline size_t GaussianKernel::getSize() const
{
    return _n;
}

}
