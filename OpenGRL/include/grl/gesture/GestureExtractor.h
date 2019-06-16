#pragma once

#include <grl/camera/DepthCamera.h>

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>

namespace grl {

/**
 * Voxel representing single entitiy in the depth image.
 * It describes coordinates X and Y and Z, which is the depth.
 */
struct Voxel
{
    Vec3i coords;
};

/**
 * Class representing object extracted from the depth image. It is a 2D object
 * with the depth.
 */
class DepthObject
{
public:
    /**
     * Basic constructor initializing state of the object.
     * After calling constructor, the depth object is empty.
     */
    DepthObject();

    /**
     * Put a single Voxel into the object - it will become part of the object
     * structure.
     * Calling this function is not simply adding Voxel to the vector, it is
     * also recaulcuating size of the object.
     *
     * @param voxel voxel, which should be added to the object strucutre.
     */
    void putVoxel(const Voxel *voxel);

    /**
     * Get number of voxels which are creating an object.
     *
     * @returns number of voxels, which are creating the object.
     */
    size_t getSize() const;

    /**
     * Clear all voxels, images etc. After call to this function the object
     * will be empty, without any voxels.
     */
    void reset();

    /**
     * Get Voxels which are creating the object. Note, that the Voxels won't be
     * sorted in any way and are stored in the order that they were put into the
     * object.
     *
     * @returns vector containing voxels creating object.
     */
    const std::vector<const Voxel *> & getVoxels() const;

    /**
     * Indicates how good was extraction of the object, where 0 is no extraction
     * at all and rest of the values is specific to the implementation of the
     * extractor object. The value must be within range of uint8_t, so between
     * 0 (the worst) and 255 (the best).
     *
     * @return accuracy of the extraction
     */
    uint8_t getAccuracy() const;

    /**
     * Sets how good the extraction of the object is. 0 means no extraction, 255
     * means best extraction.
     *
     * @param accuracy accuracy of the extraction
     */
    void setAccuracy(uint8_t accuracy);

    /**
     * Minimum depth value in the Voxels creating object.
     *
     * @returns minimum depth value in the Voxels creating object.
     */
    int getMinDepthValue() const;

    /**
     * Maximum depth value in the Voxels creating object.
     *
     * @returns maximum depth value in the Voxels creating object.
     */
    int getMaxDepthValue() const;

    /**
     * Range of all depth values of the Voxels creating object.
     *
     * @param[out] minDepth minimum depth value of the Voxels.
     * @param[out] maxDepth maximum depth value of the Voxels.
     */
    void getDepthValuesRange(int &minDepth, int &maxDepth) const;

    /**
     * Get 2D bounding box of the object, so in general it's width and height.
     * It is relative to the space from which the object was extracted, so if
     * the input Voxels were in the world space, the boudning box will be also
     * in the world space.
     *
     * @returns bounding box of the object.
     */
    const cv::Rect & getBoundingBox() const;

    /**
     * Get depth image representation of the depth object. The image won't
     * be normalized or anything else, it is simply putting all input Voxels
     * into the image.
     * If the object changed since last image acquisition or the image is being
     * acquired for the first time, it will be generated.
     *
     * @returns depth image with the object.
     */
    const cv::Mat & getDepthImageOfObject();

private:
    // Voxels, which are creating object
    std::vector<const Voxel *> _voxels;

    // Depth image of the object
    cv::Mat _depthImage;
    // Indicates, that object changed since last image was generated
    bool _objectChanged;

    // Minimum depth value of the object
    int _minDepth = INT_MAX;
    // Maximum depth value of the object
    int _maxDepth = INT_MIN;

    // Bounding box of the object in the image
    cv::Rect _boundingBox;
    // Variable for determining width of the object
    int _maxX = INT_MIN;
    // Variable for determining height of the object
    int _maxY = INT_MIN;

    // How good is the detection of the object
    uint8_t _accuracy;

    // Recalculate both bounding box and the depth with the given input voxel.
    void recalculate3DBoundingBox(const Voxel *voxel);
    // Generate
    void generateImage();
};

/**
 * Configuration of the extractor. As for now, it is empty but configurations
 * of the inherited classes should be using this struct as a parent.
 */
struct ExtractorConfig {};

/**
 * Interface, which should be used for all tools and algorithms that will be
 * implementing extraction of the hand using image and/or skeleton data.
 */
class GestureExtractor
{
public:
    /**
     * Initialize the extractor object. For now, it is not doing nothing.
     *
     * @param config configuration for the object.
     */
    bool init(const ExtractorConfig &config);

    /**
     * Try to extract both hands from the image with the given input skeleton
     * data.
     * Result of the extraction can be checked using the accurracy parameter of
     * the DepthObject.
     *
     * @param[in] depthImage image, from which the hands have to be extracted.
     * @param[in] skeleton skeleton of the person detected on the image.
     * @param[out] leftHand object with voxels creating left hand.
     * @param[out] rightHand object with voxels creating right hand.
     */
    void extractHands(const cv::Mat &depthImage,
                      const Skeleton &skeleton,
                      DepthObject &leftHand,
                      DepthObject &rightHand);

protected:
    // Helper enumeration, dictating which hand: left or right, have to be
    // extracted.
    enum Side {
        Left,
        Right
    };

    /**
     * Checks, if the algorithm has all required data to begin extraction. Upon
     * true, the hand will be extracted, upon false, extraction will be skipped.
     * This method must be overriden by the child classes.
     *
     * @param side hand which must be checked, left or right.
     * @param depthImage image, which can be used for hand extraction.
     * @param skeleton skeleton of the person which was detected on the image
     * and can be used for hand extraction.
     *
     * @returns true if the data is good enough to start extraction.
     */
    virtual bool isHandValid(Side side, const cv::Mat &depthImage, const Skeleton &skeleton) const = 0;

    /**
     * Extract hand from the image using the depth image and the skeleton of the
     * person.
     *
     * @param[in] side hand which must be extracted, left or right.
     * @param[in] depthImage image, which can be used for hand extraction.
     * @param[in] skeleton skeleton of the person which was detected on the image
     * and can be used for hand extraction.
     * @param[out] hand object representing hand which was extracted.
     */
    virtual void extractHand(Side side,
                             const cv::Mat &depthImage,
                             const Skeleton &skeleton,
                             DepthObject &hand) = 0;
};

}
