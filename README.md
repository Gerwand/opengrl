# OpenGRL - the Open Gesture Recognition Library

## Description

OpenGRL is a library, which can be used for gesture recognition using various
depth sensors. It facilitates many algorithms for both gesutres recognition and
for acquring depth images and data.

### Features

* Flexible objective interface for adding support for devices
* Easy to use API for end users
* Implementing support for different depth devices using single interface
* Skeleton based online gesture recognition
* AI used for offline gesture extraction

#### Devices

List of currently supported devices:
* Kinect 2.0

#### Gestures

The gesture recognition problem can be split into two categories

1. Online gesture recognition
    * Tracks recording (complex hands movement)
    * Hand tracing (relative hand movement for simple operations)
2. Offline gesture recognition
    * Hand and fingers position for detecting pattern in single frame

##### Extraction

Below is the description and list of algorithms used for extracting gestures
from the depth image.

1. Extracting hand from the depth image
    - *Modified flood-fill with the plane clipping and using skeleton data* -
      this algorith is localising hand using the skeleton data, which may be
      provided by the depth sensor. Then, it is using and flood fill to extract
      the hand, but it is limited by planes which are clippig arm near wrist in
      3D space.
2. Tracks recording
    - *Discretizing track* - the data is organized into the queue, and it is
      being pushed out and in every next frame with the limited buffer size.
      The track is being discretized and into equal length segments.
      The every point of the track is being compared with other track to
      determine similarity.
3. Hand tracing
    - *Use hand center from step 1. to detect hand movement* - it is returning
      raw dx, dy every frame, like mouse coursor movement.
4. Gesture extraction
    - *Extract the hand skeleton using the RDF with classified hand zones* - the
      3D model was used for learning Random Decision Forest model in various
      poses and rotations. The skeleton is being extracted, by calculating
      mass center of each zone taking into consideration weight for each class
      for every pixel.
5. Gesture comparison
    - *WIP*

## Dependencies

Please install and build below libraries:
Alternatively, they can be downloaded as a zip from: [not working](127.0.0.1)
* OpenCV (tested with v)
* QT (tested with v)
* OpenCL 

To make the project build, below environment variables must be set:
* OPENCV_DIR - path to 
* QT_DIR - path to
* CL_DIR - path to

## How to setup and build

TODO

## List and description of all subprojects

TODO

## Usage guide (for now, overview only)

TODO
