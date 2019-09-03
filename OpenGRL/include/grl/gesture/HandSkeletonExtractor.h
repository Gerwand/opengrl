#pragma once

#include <grl/gesture/HandSkeleton.h>
#include <grl/gesture/GestureExtractor.h>

namespace grl {

class HandSkeletonExtractor
{
public:
    virtual ~HandSkeletonExtractor();

    virtual void destroy();

    virtual void extractSkeleton(const grl::DepthObject &hand, grl::HandSkeleton &handSkeleton) = 0;

protected:

    bool init();
};

}
