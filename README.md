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
      The track is being discretized creating always the same number of segments.
      The every segment orientation is being compared with each other to compare
      similarity. The KNN classifier is used.
3. Hand tracing
    - *Use hand center from step 1. to detect hand movement* - it is returning
      raw dx, dy every frame, like mouse coursor movement.
4. Gesture extraction
    - *Extract the hand skeleton using the RDF with classified hand zones* - the
      3D model was used for learning Random Decision Forest model in various
      poses and rotations. The skeleton is being extracted, by calculating
      density mode of each zone taking into consideration weight for each class
      for every pixel.
5. Gesture comparison
    - The orientation of each bone is being calculated. It is being compared to
      other gesture orientation and distance from each other is used to get the
      unsimilarit. Then, the KNN is being used for classification.

## Dependencies

Please install and build below libraries:
* OpenCV (tested with v)
* QT (tested with v)
* OpenCL

To make the project build, below environment variables must be set:
* OPENCV_DIR - path to
* QT_DIR - path to
* CL_DIR - path to

## How to setup and build

VS preparation

1. Install Visual Studio 2017 - v141 was used for development
1. Install [Windows 10 SDK 10.0.17763.0](
   https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk) -
   this step is needed for building this VS project

Qt setup for **OpenGRL_GUI** project

1. Install [Qt 5.12.2](https://www.qt.io/download) - VS 2017 64bit binaries
    * It can be set in the installer
    * Alternatively, the Qt can be built from source
    * One can also download the binaries from [here](127.0.0.1)
    * Next, the envirnoment variable QTDIR must be set. It should indicate
      location of the Qt `lib`, `bin` and `include` folders, like this:\
        `C:\Qt\5.12.2\msvc2017_64`
1. Install `Qt Visual Studio Tools` extension for VS 2017
1. In `Qt VS Tools -> Qt Options` add Qt path and set the default version

OpenCV setup

1. The OpenCV must be built from scratch. First, download the [sources](
    https://github.com/opencv/opencv/archive/3.4.5.zip).
1. Extract and open it using [CMake](https://cmake.org/download/)
1. Specify build target and click `Configure`
1. Set `WITH_QT` and `WITH_OPENGL`
1. Click `Configure` again and set variables `QT_QMAKE_EXECUTABLE` to
   `%QTDIR%\bin\qmake.exe` and `Qt5Concurrent_DIR`, `Qt5Core_DIR`, `Qt5Gui_DIR`,
   `Qt5Test_DIR`, `Qt5Widgets_DIR`, `Qt5OpenGL_DIR` to `%QTDIR%\lib\cmake\X`,
   where `X` is the variable name without `DIR` appendix. Either the absolute
   path must be provided or the folder selection window can be used, which is
   supporting usage of enviroment variables. Note, it may be required to hit the
   Configure multiple times to make show up all of the variables.
1. Click configure again to make it build without errors.
1. After configuring successfully, click generate
1. Open the location where the project was generated and open `OpenCV.sln`
1. Now execute build for `INSTALL` in `Debug` and `Release` config on `x64`
1. In the build folder, the `install` will be created. Create new folder for
   default OpenCV directory somewhere and copy `install/include` and
   `install/x64` to the newly created folder.
1. Point the env variable to this folder and call it `OPENCV_DIR`

Kinect setup

1. Download Kinect SDK from [there](
   https://www.microsoft.com/en-us/download/details.aspx?id=44561)
1. Install it

## List and description of all subprojects

**OpenGRL** - this is the main library. It contains all of the algorithms and
main code. It is being built to the library file, so it can be exported and
headers can be copied from the include directory of the subproject to use it
in the separate app standalone.

**OpenGRL_UnitTests** - all unit tests were written there. It checks if the
basic algorithms works by testing different scenarios.

**OpenGRL_GUI** - GUI implementing track and gesture extraction + recognition
using the sample database. It can be also used to capture and save the gesture
or the track.

**OpenGRL_RDF_Tester** - standalone app, at first used to test if the RDF is
working fine. Then, it's behavior was extended to check the efficiency of the
RDF classification and the KNN classification for both the tracks and the hands.
