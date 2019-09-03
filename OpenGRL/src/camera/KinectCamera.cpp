#include <grl/camera/KinectCamera.h>

#include <iostream>

namespace grl {

template<typename T>
inline void
safeRelease(T& COMptr)
{
    if (COMptr != nullptr) {
        COMptr->Release();
        COMptr = nullptr;
    }
}

int
KinectCamera::init()
{
    cv::Mat image;
    cv::Mat desc;
    std::vector<cv::KeyPoint> kp;
    HRESULT hr;

    if (isValid()) {
        DEBUG_ERR("Device already initialized!");
        return EINVAL;
    }

    // Get Kinect sensor pointer
    hr = GetDefaultKinectSensor(&_sensor);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get kinect sensor, %ld", hr);
        return hr;
    }

    if (_sensor == nullptr) {
        DEBUG_ERR("No kinect device found");
        return ENODEV;
    }

    // Open kinect sensor
    hr = _sensor->Open();
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to open kinect sensor, %ld", hr);
        goto err_sensor_release;
    }

    // Depth frame source from the kinect sensor
    IDepthFrameSource *depthFrameSource;
    hr = _sensor->get_DepthFrameSource(&depthFrameSource);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get deph from source, %ld", hr);
        goto err_sensor_release;
    }

    // Reader, that allows to read depth sensor from the kinect device
    hr = depthFrameSource->OpenReader(&_depthFrameReader);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to open deph frame reader, %ld", hr);
        goto err_depths_release;
    }

    // Get width and heigh of an image
    IFrameDescription *frameDesc;
    hr = depthFrameSource->get_FrameDescription(&frameDesc);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get deph frame descriptor, %ld", hr);
        goto err_depthr_release;
    }
    frameDesc->get_Width(&_depthWidth);
    frameDesc->get_Height(&_depthHeight);

    _depthBuffer = new uint16_t[_depthWidth * _depthHeight];
    if (_depthBuffer == nullptr) {
        DEBUG_ERR("Could not allocate memory for depth buffer!");
        hr = ENOMEM;
        goto err_frame_release;
    }

    // Get body frame resources
    hr = _sensor->get_CoordinateMapper(&_coordMapper);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get coordinate mapper, %ld", hr);
        goto err_buf_release;
    }

    IBodyFrameSource *bodyFrameSource;
    hr = _sensor->get_BodyFrameSource(&bodyFrameSource);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get body frame source, %ld", hr);
        goto err_mapper_release;
    }

    hr = bodyFrameSource->OpenReader(&_bodyFrameReader);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get body frame readers, %ld", hr);
        goto err_bodys_release;
    }

    IColorFrameSource *colorFrameSource;
    hr = _sensor->get_ColorFrameSource(&colorFrameSource);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get body frame readers, %ld", hr);
        goto err_bodyr_release;
    }

    hr = colorFrameSource->OpenReader(&_colorFrameReader);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get body frame reader, %ld", hr);
        goto err_colors_release;
    }


    IFrameDescription *frameDescription;
    colorFrameSource->CreateFrameDescription(ColorImageFormat_Bgra, &frameDescription);
    int colorWidth, colorHeight;
    frameDescription->get_Width(&colorWidth);
    frameDescription->get_Height(&colorHeight);
    _lastColorFrame = cv::Mat(
        colorHeight,
        colorWidth,
        CV_8UC4
    );

    // Release no longer needed resouces
    safeRelease(frameDescription);
    safeRelease(colorFrameSource);
    safeRelease(bodyFrameSource);
    safeRelease(depthFrameSource);
    safeRelease(frameDesc);

    // Set size of an image as constant
    _lastFrame = cv::Mat::zeros(cv::Size(_depthWidth, _depthHeight), CV_16UC1);

    setValid(true);

    return 0;

err_colors_release:
    safeRelease(colorFrameSource);
err_bodyr_release:
    safeRelease(_bodyFrameReader);
err_bodys_release:
    safeRelease(bodyFrameSource);
err_mapper_release:
    safeRelease(_coordMapper);
err_buf_release:
    delete[] _depthBuffer;
err_frame_release:
    safeRelease(frameDesc);
err_depthr_release:
    safeRelease(_depthFrameReader);
err_depths_release:
    safeRelease(depthFrameSource);
err_sensor_release:
    safeRelease(_sensor);
    return hr;
}

void
KinectCamera::destroy()
{
    if (!isValid()) {
        DEBUG_ERR("Device already destroyed!");
        return;
    }

    setValid(false);
    delete[] _depthBuffer;
    safeRelease(_depthFrameReader);
    safeRelease(_sensor);
}

bool KinectCamera::updateColorFrame()
{
    HRESULT hr;

    IColorFrame* colorFrame;
    // Check if any new frame available
    hr = _colorFrameReader->AcquireLatestFrame(&colorFrame);
    if (FAILED(hr)) // Exit if not frame available
        return false;

    hr = colorFrame->CopyConvertedFrameDataToArray(
        _lastColorFrame.cols * _lastColorFrame.rows * 4,
        _lastColorFrame.data,
        ColorImageFormat_Rgba
    );

    safeRelease(colorFrame); // Depth frame no longer needed


    return true;
}

bool
KinectCamera::getFrame(cv::Mat *image)
{
    updateColorFrame();

    HRESULT hr;

    IDepthFrame* depthFrame;
    // Check if any new frame available
    hr = _depthFrameReader->AcquireLatestFrame(&depthFrame);
    if (FAILED(hr)) // Exit if not frame available
        return false;

    // Copy data from the depth frame to the buffer
    hr = depthFrame->CopyFrameDataToArray(_depthWidth * _depthHeight, _depthBuffer);
    safeRelease(depthFrame); // Depth frame no longer needed
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to copy data from the depth frame, %ld", hr);
        return false;
    }

    // Copy depth buffer to the predefined image (avoid constant resizing of
    // the iamge that was passed as argument)
    // Note that 1 unit is equal to 1mm of distance.
    // The minimum allowed value is 500mm, and maximal is 8000mm (0.5m and 8m)
    // Every point outside this range will be equal to 0
    uint16_t* pixel = _depthBuffer;
    for (auto it = _lastFrame.begin<uint16_t>();
         it != _lastFrame.end<uint16_t>(); ++it) {
        *it = *pixel++;
    }

    // Set image as reference to last frame (should not hurt us, because it is only
    // used as matrix with constant size to avoid resizing and allocating memory every
    // frame)
    *image = _lastFrame;

    return true;
}

bool
KinectCamera::getSkeletons(Skeletons *skeletons)
{
    // Map the Kinect joints to OGRL joints
    static JointType typesMap[JointType_Count] = {
        JointType::SPINE_BASE,      // JointType_SpineBase = 0,
        JointType::COUNT,           // JointType_SpineMid = 1,
        JointType::COUNT,           // JointType_Neck = 2,
        JointType::COUNT,           // JointType_Head = 3,
        JointType::LEFT_SHOULDER,   // JointType_ShoulderLeft = 4,
        JointType::LEFT_ELBOW,      // JointType_ElbowLeft = 5,
        JointType::LEFT_WRIST,      // JointType_WristLeft = 6,
        JointType::LEFT_HAND,       // JointType_HandLeft = 7,
        JointType::RIGHT_SHOULDER,  // JointType_ShoulderRight = 8,
        JointType::RIGHT_ELBOW,     // JointType_ElbowRight = 9,
        JointType::RIGHT_WRIST,     // JointType_WristRight = 10,
        JointType::RIGHT_HAND,      // JointType_HandRight = 11,
        JointType::COUNT,           // JointType_HipLeft = 12,
        JointType::COUNT,           // JointType_KneeLeft = 13,
        JointType::COUNT,           // JointType_AnkleLeft = 14,
        JointType::COUNT,           // JointType_FootLeft = 15,
        JointType::COUNT,           // JointType_HipRight = 16,
        JointType::COUNT,           // JointType_KneeRight = 17,
        JointType::COUNT,           // JointType_AnkleRight = 18,
        JointType::COUNT,           // JointType_FootRight = 19,
        JointType::SPINE_SHOULDERS, // JointType_SpineShoulder = 20,
        JointType::COUNT,           // JointType_HandTipLeft = 21,
        JointType::COUNT,           // JointType_ThumbLeft = 22,
        JointType::COUNT,           // JointType_HandTipRight = 23,
        JointType::COUNT,           // JointType_ThumbRight = 24,
    };
    IBodyFrame *bodyFrame;
    HRESULT hr = _bodyFrameReader->AcquireLatestFrame(&bodyFrame);

    if (FAILED(hr))
        return false;

    // Get bodies inside the array
    IBody* bodies[BODY_COUNT] = { 0 };
    hr = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, bodies);
    safeRelease(bodyFrame); // Body frame is no longer needed
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get body data, %ld", hr);
        return false;
    }

    // Process data
    for (int i = 0; i < BODY_COUNT; ++i) {
        IBody* body = bodies[i];
        if (body != nullptr) {
            BOOLEAN tracked = false;
            hr = body->get_IsTracked(&tracked);
            if (FAILED(hr) || !tracked)
                continue;

            // Lean from kinect is returning strange values, so calculate custom one
            // PointF lean;
            // hr = body->get_Lean(&lean);
            // if (FAILED(hr))
            //	continue;

            Skeleton skeleton;
            //skeleton.lean = std::max(abs(lean.X), abs(lean.Y)); // Get maximum absolute lean

            // Todo: test lean parameter and do not add object if it is too high

            ::Joint joints[JointType_Count];
            hr = body->GetJoints(JointType_Count, joints);
            if (FAILED(hr))
                continue;

            JointOrientation jointOrientations[JointType_Count];
            hr = body->GetJointOrientations(JointType_Count, jointOrientations);
            if (FAILED(hr))
                continue;

            CameraSpacePoint cameraPoints[JointType::COUNT];
            DepthSpacePoint depthPoints[JointType::COUNT];

            for (int j = 0; j < JointType_Count; ++j) {
                JointType type = typesMap[joints[j].JointType];
                if (type == JointType::COUNT)
                    continue;

                Joint &grlJoint = skeleton.joints[type];

                grlJoint.tracked = joints[j].TrackingState == TrackingState_Tracked;
                grlJoint.type = type;
                grlJoint.coordWorld = {
                    joints[j].Position.X,
                    joints[j].Position.Y,
                    joints[j].Position.Z
                };
                // Convert depth coordinates to image
                cameraPoints[type] = joints[j].Position;

                grlJoint.orientation = {
                    jointOrientations[j].Orientation.x,
                    jointOrientations[j].Orientation.y,
                    jointOrientations[j].Orientation.z };
            }
            // Map camera coordinates to depth camera space.
            // Do it at once for better performance.
            _coordMapper->MapCameraPointsToDepthSpace(JointType::COUNT, cameraPoints, JointType::COUNT, depthPoints);
            for (int j = 0; j < JointType::COUNT; ++j)
                skeleton.joints[j].coordDepthImage = {
                    static_cast<int>(depthPoints[j].X),
                    static_cast<int>(depthPoints[j].Y) };

            // Calculate lean basing on spine
            Vec3f spineBaseCoords = skeleton.joints[SPINE_BASE].coordWorld;
            Vec3f spineShouldersCoords = skeleton.joints[SPINE_SHOULDERS].coordWorld;

            Vec3f spine = {
                spineShouldersCoords.x - spineBaseCoords.x,
                spineShouldersCoords.y - spineBaseCoords.y,
                spineShouldersCoords.z - spineBaseCoords.z,
            };
            float spineLenXY = sqrt(spine.x*spine.x + spine.y*spine.y);
            float spineLenZY = sqrt(spine.z*spine.z + spine.y*spine.y);

            float leanX = acos(spine.y/spineLenXY);
            float leanZ = acos(spine.y/spineLenZY);
            skeleton.lean = std::max(leanX, leanZ) * 180.0f / static_cast<float>(M_PI);

            skeleton.distance = skeleton.joints[JointType::SPINE_SHOULDERS].coordWorld.z;

            skeletons->push_back(skeleton);
        }
    }

    // Release unnecessary data
    for (int i = 0; i < BODY_COUNT; ++i)
        safeRelease(bodies[i]);

    return true;
}

bool
KinectCamera::worldToImage(const std::vector<Vec3f>& world, std::vector<Vec2f>& image) const
{
    if (_coordMapper == nullptr)
        return false;

    image.resize(world.size());

    // The CameraSpacePoint definition is same as Vec3f, so it can be safely reinterpreted
    const CameraSpacePoint *cameraPoints = reinterpret_cast<const CameraSpacePoint *>(world.data());
    // Same for DepthSpacePoint which is the same as Vec2f
    DepthSpacePoint *depthPoints = reinterpret_cast<DepthSpacePoint *>(image.data());

    // Map the points
    _coordMapper->MapCameraPointsToDepthSpace(static_cast<UINT>(world.size()),
                                              cameraPoints,
                                              static_cast<UINT>(image.size()),
                                              depthPoints);

    return true;
}

bool
KinectCamera::worldToImage(Vec3f world, Vec2f &image) const
{
    if (_coordMapper == nullptr)
        return false;

    // The CameraSpacePoint definition is same as Vec3f, so it can be safely reinterpreted
    const CameraSpacePoint &cameraPoint = reinterpret_cast<const CameraSpacePoint &>(world);
    // Same for DepthSpacePoint which is the same as Vec2f
    DepthSpacePoint *depthPoint = reinterpret_cast<DepthSpacePoint *>(&image);

    // Map the point
    _coordMapper->MapCameraPointToDepthSpace(cameraPoint, depthPoint);

    return true;
}

bool KinectCamera::imageToWorld(const std::vector<Vec2f> &image, const std::vector<uint16_t> &depths, std::vector<Vec3f> &world) const
{
    if (_coordMapper == nullptr)
        return false;

    world.resize(image.size());

    // The CameraSpacePoint definition is same as Vec3f, so it can be safely reinterpreted
    CameraSpacePoint *cameraPoints = reinterpret_cast<CameraSpacePoint *>(world.data());
    // Same for DepthSpacePoint which is the same as Vec2f
    const DepthSpacePoint *depthPoints = reinterpret_cast<const DepthSpacePoint *>(image.data());

    // Map the points
    _coordMapper->MapDepthPointsToCameraSpace(static_cast<UINT>(image.size()),
                                              depthPoints,
                                              static_cast<UINT>(depths.size()),
                                              static_cast<const uint16_t *>(depths.data()),
                                              static_cast<UINT>(world.size()),
                                              cameraPoints);

    return true;
}

bool KinectCamera::imageToWorld(Vec3f &world, uint16_t depth, Vec2f image) const
{
    if (_coordMapper == nullptr)
        return false;

    // The CameraSpacePoint definition is same as Vec3f, so it can be safely reinterpreted
    CameraSpacePoint *cameraPoint = reinterpret_cast<CameraSpacePoint *>(&world);
    // Same for DepthSpacePoint which is the same as Vec2f
    DepthSpacePoint &depthPoint = reinterpret_cast<DepthSpacePoint &>(image);

    // Map the point
    _coordMapper->MapDepthPointToCameraSpace(depthPoint, depth, cameraPoint);

    return true;
}


void KinectCamera::dumpStats()
{
    HRESULT hr;

    IDepthFrameSource *depthFrameSource;
    hr = _sensor->get_DepthFrameSource(&depthFrameSource);
    if (FAILED(hr)) {
        DEBUG_ERR("Failed to get deph from source, %ld", hr);
        return;
    }

    std::cout << "Width: " << _depthWidth << " height: " << _depthHeight << std::endl;

    uint16_t maxDist, minDist;
    depthFrameSource->get_DepthMaxReliableDistance(&maxDist);
    depthFrameSource->get_DepthMinReliableDistance(&minDist);

    std::cout << "depthmin: " << minDist<< " depthMax: " << maxDist << std::endl;

    safeRelease(depthFrameSource);
}

void KinectCamera::colorizeDepth(cv::Mat &depthWithColor)
{
    std::vector<ColorSpacePoint> colorPoints(_depthWidth * _depthHeight);

    _coordMapper->MapDepthFrameToColorSpace(
        _depthWidth * _depthHeight,
        _depthBuffer,
        _depthWidth * _depthHeight,
        colorPoints.data()
     );

    depthWithColor = cv::Mat::zeros(_depthHeight, _depthWidth, CV_8UC4);
    auto itColorized = depthWithColor.begin<cv::Vec4b>();
    for (auto it = colorPoints.begin(); it != colorPoints.end(); ++it, ++itColorized) {
        if (it->X != -std::numeric_limits<float>::infinity() &&
            it->Y != -std::numeric_limits<float>::infinity() &&
            it->X > 0 &&
            it->Y > 0)
            *itColorized = _lastColorFrame.at<cv::Vec4b>(cv::Point(
                static_cast<int>(std::round(it->X)),
                static_cast<int>(std::round(it->Y))
        ));
    }
}

}