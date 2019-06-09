#include <grl/utils/math/GaussianKernel.h>

namespace grl {

GaussianKernel::GaussianKernel(float bandwidth, size_t size)
{
    // Kernel size must be odd
    assert(size & 1);
    _n = size;
    _mask.reserve(size);

    int mid = static_cast<int>(_n / 2);
    for (int y = 0; y < _n; ++y) {
        for (int x = 0; x < _n; ++x) {
            Vec2i coords{ (x-mid), (y-mid) };
            float val = coords.length() / bandwidth;
            _mask.push_back(exp(-val*val));
        }
    }
}

}
