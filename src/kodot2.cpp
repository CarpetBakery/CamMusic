#include "kodot2.h"

#include "util.h"
#include "windowsdef.h"
#include <Kinect.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


void godot::Kodot2::_bind_methods()
{
    // Expose methods to GDScript
    ClassDB::bind_method(D_METHOD("kinect_init"), &Kodot2::kinectInitialize);
    ClassDB::bind_method(D_METHOD("kinect_update"), &Kodot2::kinectUpdate);
    ClassDB::bind_method(D_METHOD("get_body_joint_positions_3d", "bodyId"), &Kodot2::getBodyJointPositions3D, DEFVAL(-1));
    ClassDB::bind_method(D_METHOD("get_body_joint_positions_2d", "bodyId"), &Kodot2::getBodyJointPositions2D, DEFVAL(-1));

    ClassDB::bind_method(D_METHOD("get_first_tracked_body"), &Kodot2::getFirstTrackedBody);
    ClassDB::bind_method(D_METHOD("get_tracked_bodies"), &Kodot2::getTrackedBodies);
    ClassDB::bind_method(D_METHOD("get_all_bodies"), &Kodot2::getAllBodies);

    // -- Get/set --
    ClassDB::bind_method(D_METHOD("get_tracked_body_count"), &Kodot2::getTrackedBodyCount);

    // -- Exports --
    ClassDB::bind_method(D_METHOD("get_printVerboseErrors"), &Kodot2::get_printVerboseErrors);
	ClassDB::bind_method(D_METHOD("set_printVerboseErrors", "p_printVerboseErrors"), &Kodot2::set_printVerboseErrors);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "printVerboseErrors"), "set_printVerboseErrors", "get_printVerboseErrors");

    ClassDB::bind_method(D_METHOD("get_screenSize"), &Kodot2::get_screenSize);
	ClassDB::bind_method(D_METHOD("set_screenSize", "p_screenSize"), &Kodot2::set_screenSize);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "screenSize"), "set_screenSize", "get_screenSize");
    

    // Joints enum
    BIND_ENUM_CONSTANT(SpineBase);
    BIND_ENUM_CONSTANT(SpineMid);
    BIND_ENUM_CONSTANT(Neck);
    BIND_ENUM_CONSTANT(Head);
    BIND_ENUM_CONSTANT(ShoulderLeft);
    BIND_ENUM_CONSTANT(ElbowLeft);
    BIND_ENUM_CONSTANT(WristLeft);
    BIND_ENUM_CONSTANT(HandLeft);
    BIND_ENUM_CONSTANT(ShoulderRight);
    BIND_ENUM_CONSTANT(ElbowRight);
    BIND_ENUM_CONSTANT(WristRight);
    BIND_ENUM_CONSTANT(HandRight);
    BIND_ENUM_CONSTANT(HipLeft);
    BIND_ENUM_CONSTANT(KneeLeft);
    BIND_ENUM_CONSTANT(AnkleLeft);
    BIND_ENUM_CONSTANT(FootLeft);
    BIND_ENUM_CONSTANT(HipRight);
    BIND_ENUM_CONSTANT(KneeRight);
    BIND_ENUM_CONSTANT(AnkleRight);
    BIND_ENUM_CONSTANT(FootRight);
    BIND_ENUM_CONSTANT(SpineShoulder);
    BIND_ENUM_CONSTANT(HandTipLeft);
    BIND_ENUM_CONSTANT(ThumbLeft);
    BIND_ENUM_CONSTANT(HandTipRight);
    BIND_ENUM_CONSTANT(ThumbRight);
    BIND_ENUM_CONSTANT(Count);

    // HandState enum
    BIND_ENUM_CONSTANT(Unknown);
    BIND_ENUM_CONSTANT(NotTracked);
    BIND_ENUM_CONSTANT(Open);
    BIND_ENUM_CONSTANT(Closed);
    BIND_ENUM_CONSTANT(Lasso);
}

void godot::Kodot2::_notification(int p_what)
{
    // For executing stuff in _ready and _process alongside GDScript
    // switch (p_what)
    // {
    // case Node::NOTIFICATION_READY:
    //     kinectInitialize();
    //     break;
    // case Node::NOTIFICATION_PROCESS:
    //     kinectUpdate();
    //     break;
    // }
}

bool godot::Kodot2::kinectInitialize()
{
    print_line("Initializing Kinect 2...");
    isKinectInitialized = true;

    HRESULT hr;
    hr = GetDefaultKinectSensor(&kinectSensor);
    if (FAILED(hr))
    {
        return true;
    }

    if (kinectSensor)
    {
        // Initialize the Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* pBodyFrameSource = NULL;
        hr = kinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = kinectSensor->get_CoordinateMapper(&coordMapper);
        }

        if (SUCCEEDED(hr))
        {
            hr = kinectSensor->get_BodyFrameSource(&pBodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrameSource->OpenReader(&bodyFrameReader);
        }

        SafeRelease(pBodyFrameSource);
    }

    ERR_FAIL_COND_V_MSG(!kinectSensor || FAILED(hr), true, "No ready Kinect found!");

    // Populate kodotBodies
    for (int i = 0; i < BODY_COUNT; i++)
    {
        kodotBodies.push_back(memnew(Kodot2Body));
    }

    print_line("Kinect 2 initialized");
    return false;
}

void godot::Kodot2::kinectUpdate()
{
    ERR_FAIL_COND_MSG(!isKinectInitialized, "Trying to update when Kinect was never initialized. Try calling `kinect_init' in your ready function.");

    if (!bodyFrameReader)
    {
        return;
    }

    IBodyFrame* bodyFrame = NULL;
    HRESULT hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

    if (SUCCEEDED(hr))
    {
        // NOTE: We don't really need the timestamp stuff
        // might remove this later
        INT64 time = 0;
        hr = bodyFrame->get_RelativeTime(&time);

        // Make sure to initialize memory with default value
        IBody* iBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            // Seems like _countof is just a constant value...'
            // Might just replace with BODY_COUNT later
            // UPDATE: I'm not so sure of this now
            hr = bodyFrame->GetAndRefreshBodyData(_countof(iBodies), iBodies);
        }

        if (SUCCEEDED(hr))
        {
            processBodies(time, _countof(iBodies), iBodies);
        }

        // Make sure to free the iBodies
        for (int i = 0; i < _countof(iBodies); i++)
        {
            SafeRelease(iBodies[i]);
        }
    }

    SafeRelease(bodyFrame);
}

void godot::Kodot2::processBodies(uint64_t nTime, int bodyCount, IBody** iBodies)
{
    // Reset tracking flags on kodot bodies
    for (int i = 0; i < bodyCount; i++)
    {
        Kodot2Body* body = cast_to<Kodot2Body>(kodotBodies.get(i));
        body->tracked = false;
    }
    trackedBodyCount = 0;

    HRESULT hr;
    TypedArray<Vector2> jointPoints;

    for (int i = 0; i < bodyCount; i++)
    {        
        IBody* iBody = iBodies[i];
        if (!iBody)
        {
            continue;
        }

        BOOLEAN tracked = false;
        hr = iBody->get_IsTracked(&tracked);
        if (!(SUCCEEDED(hr) && tracked))
        {
            continue;
        }
        
        // Body is valid and tracked - get joints + hand states
        Joint joints[JointType_Count];
        HandState leftHandState = HandState_Unknown;
        HandState rightHandState = HandState_Unknown;

        hr = iBody->GetJoints(_countof(joints), joints);
        if (!SUCCEEDED(hr))
        {
            continue;
        }

        // We were able to get this body's joints; set this body as tracked
        Kodot2Body* body = cast_to<Kodot2Body>(kodotBodies.get(i));
        body->tracked = true;

        // Add joint positions to this body's joints
        for (int k = 0; k < JointType_Count; k++)
        {
            CameraSpacePoint jPos = joints[k].Position;

            body->joints.set(k, Vector3(
                jPos.X,
                jPos.Y,
                jPos.Z
            ));
            body->joints2D.set(k, bodyToScreen(body->joints.get(k)));
        }

        // Update hand states
        iBody->get_HandLeftState(&leftHandState);
        iBody->get_HandRightState(&rightHandState);
        body->leftHandState = static_cast<int>(leftHandState);
        body->rightHandState = static_cast<int>(rightHandState);

        trackedBodyCount++;
    }
}

godot::Kodot2Body* godot::Kodot2::getBody(int _bodyIndex)
{
    // Don't try to use an index out of range
    // Returns first body if this failed
    ERR_FAIL_COND_V_MSG(_bodyIndex < 0 || _bodyIndex > BODY_COUNT - 1, cast_to<Kodot2Body>(kodotBodies.front()), "Trying to get body index outside range.");
    
    return cast_to<Kodot2Body>(kodotBodies.get(_bodyIndex));
}

godot::TypedArray<godot::Kodot2Body> godot::Kodot2::getAllBodies()
{
    return kodotBodies;
}

godot::Kodot2Body* godot::Kodot2::getFirstTrackedBody()
{
    size_t len = kodotBodies.size();
    for (int i = 0; i < len; i++)
    {
        Kodot2Body* body = getBody(i);
        if (body->isTracked())
        {
            firstTrackedBodyIndex = i;
            return body;
        }
    }
    return getBody(firstTrackedBodyIndex);
}

godot::TypedArray<godot::Kodot2Body> godot::Kodot2::getTrackedBodies()
{
    TypedArray<Kodot2Body> _trackedBodies;

    size_t len = kodotBodies.size();
    for (int i = 0; i < len; i++)
    {
        Kodot2Body* body = getBody(i);
        if (body->isTracked())
        {
            _trackedBodies.push_back(body);
        }
    }

    return _trackedBodies;
}

godot::TypedArray<godot::Vector2> godot::Kodot2::getBodyJointPositions2D(int bodyId)
{
    Kodot2Body* body;
    if (bodyId < 0)
    {
        body = getFirstTrackedBody();
    }
    else
    {
        body = getBody(bodyId);
    }
    return body->getJointPositions2D();
}

godot::TypedArray<godot::Vector3> godot::Kodot2::getBodyJointPositions3D(int bodyId)
{
    Kodot2Body* body;
    if (bodyId < 0)
    {
        body = getFirstTrackedBody();
    }
    else
    {
        body = getBody(bodyId);
    }
    return body->getJointPositions3D();
}

godot::Vector2 godot::Kodot2::bodyToScreen(float x, float y, float z)
{
    CameraSpacePoint bodyPoint = {x, y, z};
    DepthSpacePoint depthPoint = {0};

    coordMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

    return Vector2(
        static_cast<float>(depthPoint.X * screenSize.width) / DEPTH_WIDTH,
        static_cast<float>(depthPoint.Y * screenSize.height) / DEPTH_HEIGHT
    );
}

godot::Vector2 godot::Kodot2::bodyToScreen(godot::Vector3 bodyPoint)
{
    return bodyToScreen(bodyPoint.x, bodyPoint.y, bodyPoint.z);
}


// -- Get/set --
int godot::Kodot2::getTrackedBodyCount()
{
    return trackedBodyCount;
}

// -- Exports --
void godot::Kodot2::set_printVerboseErrors(bool const p_printVerboseErrors)
{
    printVerboseErrors = p_printVerboseErrors;
}
bool godot::Kodot2::get_printVerboseErrors() const
{
    return printVerboseErrors;
}

void godot::Kodot2::set_screenSize(Vector2 const p_screenSize)
{
    screenSize = p_screenSize;
}
godot::Vector2 godot::Kodot2::get_screenSize() const
{
    return screenSize;
}


// -- Debug --
void godot::Kodot2::printErr(const godot::Variant &p_variant)
{
    if (!printVerboseErrors)
    {
        return;
    }
    print_error(p_variant);
}

godot::Kodot2::Kodot2()
{
    // GDExtension classes are constructed/destructed once when the extension is loaded
    // This seems to be the behavior even as of 2025-07-31
    // See: https://github.com/godotengine/godot/pull/82554#issuecomment-1744888107
}

godot::Kodot2::~Kodot2()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    // Cleanup
    SafeRelease(bodyFrameReader);
    SafeRelease(coordMapper);

    // Close Kinect sensor
    if (kinectSensor)
    {
        kinectSensor->Close();
    }
    SafeRelease(kinectSensor);

    print_line("Kodot2 Freed");
}