#pragma once

#include <grl/gesture/HandSkeleton.h>
#include <grl/gesture/GestureExtractor.h>

namespace grl {

// Abstract class with no functionality, which is being interface for
// hand skeleton extractors.
class HandSkeletonExtractor
{
public:
    virtual ~HandSkeletonExtractor();

    virtual void destroy();

    // Extract the skeleton using the depth object representing the hand.
    virtual void extractSkeleton(const grl::DepthObject &hand, grl::HandSkeleton &handSkeleton) = 0;

protected:

    bool init();
};

}
