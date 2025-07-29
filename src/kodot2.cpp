#include "kodot2.h"

#include "util.h"
#include "windowsdef.h"
#include <Kinect.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/time.hpp>


void godot::Kodot2::_bind_methods()
{
    // Expose methods to GDScript
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot2::initialize);
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot2::update);
    ClassDB::bind_method(D_METHOD("getBodyJointPositions3D"), &godot::Kodot2::getBodyJointPositions3D);
    ClassDB::bind_method(D_METHOD("getBodyJointPositions2D"), &godot::Kodot2::getBodyJointPositions2D);

    // -- Get/set --
    ClassDB::bind_method(D_METHOD("getTrackedBodyCount"), &godot::Kodot2::getTrackedBodyCount);

    // -- Exports --
    ClassDB::bind_method(D_METHOD("get_printVerboseErrors"), &godot::Kodot2::get_printVerboseErrors);
	ClassDB::bind_method(D_METHOD("set_printVerboseErrors", "p_printVerboseErrors"), &godot::Kodot2::set_printVerboseErrors);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "printVerboseErrors"), "set_printVerboseErrors", "get_printVerboseErrors");

    ClassDB::bind_method(D_METHOD("get_screenSize"), &godot::Kodot2::get_screenSize);
	ClassDB::bind_method(D_METHOD("set_screenSize", "p_screenSize"), &godot::Kodot2::set_screenSize);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "screenSize"), "set_screenSize", "get_screenSize");
    
    ClassDB::bind_method(D_METHOD("get_kodotBodies"), &godot::Kodot2::get_kodotBodies);
	ClassDB::bind_method(D_METHOD("set_kodotBodies", "p_kodotBodies"), &godot::Kodot2::set_kodotBodies);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "kodotBodies"), "set_kodotBodies", "get_kodotBodies");


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
}

bool godot::Kodot2::initialize()
{
    print_line("Initializing Kinect 2");

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

    if (!kinectSensor || FAILED(hr))
    {
        print_error("No ready Kinect found!");
        return true;
    }

    // Setup kodotBodies
    for (int i = 0; i < BODY_COUNT; i++)
    {
        kodotBodies.push_back(memnew(Kodot2Body));
    }

    print_line("Kinect 2 initialized");
    return false;
}

void godot::Kodot2::_exit_tree()
{
    SafeRelease(bodyFrameReader);
    SafeRelease(coordMapper);

    // Close Kinect sensor
    if (kinectSensor)
    {
        kinectSensor->Close();
    }
    SafeRelease(kinectSensor);
}

void godot::Kodot2::update(double delta)
{
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
        body->isTracked = false;
    }    

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
        
        // TODO: Do something with this information
        iBody->get_HandLeftState(&leftHandState);
        iBody->get_HandRightState(&rightHandState);

        hr = iBody->GetJoints(_countof(joints), joints);
        if (!SUCCEEDED(hr))
        {
            continue;
        }

        // We were able to get this body's joints; set this body as tracked
        Kodot2Body* body = cast_to<Kodot2Body>(kodotBodies.get(i));
        body->isTracked = true;

        // Add joint positions to this body's joints
        for (int k = 0; k < JointType_Count; k++)
        {
            CameraSpacePoint jPos = joints[k].Position;
            body->joints.set(k, Vector3(
                jPos.X,
                jPos.Y,
                jPos.Z
            ));
        }
    }
}

godot::Kodot2Body* godot::Kodot2::getFirstValidBody()
{
    for (int k = 0; k < kodotBodies.size(); k++)
    {
        Kodot2Body* body = cast_to<Kodot2Body>(kodotBodies.get(k));
        if (body->isTracked)
        {
            return body;
        }
    }
    return nullptr;
}

bool godot::Kodot2::getJoints(int bodyId, _Joint* joints)
{
    return false;
}

godot::TypedArray<godot::Vector2> godot::Kodot2::getBodyJointPositions2D()
{
    Kodot2Body* body = getFirstValidBody();
    TypedArray<Vector2> jointPoints;

    if (body == nullptr)
    {
        // print_error("No valid bodies found.");
        return jointPoints;
    }

    TypedArray<Vector3> joints = body->joints;
    for (int i = 0; i < joints.size(); i++)
    {
        jointPoints.push_back(bodyToScreen(joints.get(i)));
    }

    return jointPoints;
}

godot::TypedArray<godot::Vector3> godot::Kodot2::getBodyJointPositions3D()
{
    Kodot2Body* body = getFirstValidBody();
    if (body == nullptr)
    {
        return TypedArray<Vector3>();
    }

    return body->joints;
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
    return BODY_COUNT;
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

void godot::Kodot2::set_kodotBodies(TypedArray<Kodot2Body> const p_kodotBodies)
{
    kodotBodies = p_kodotBodies;
}
godot::TypedArray<godot::Kodot2Body> godot::Kodot2::get_kodotBodies() const
{
    return kodotBodies;
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
