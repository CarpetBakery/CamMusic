#include "kodot2.h"

#include "util.h"
#include "windowsdef.h"
#include <Kinect.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


void godot::Kodot2::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot2::initialize);
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot2::update);
    ClassDB::bind_method(D_METHOD("getBodyJointPositions3D", "bodyId"), &godot::Kodot2::getBodyJointPositions3D);
    ClassDB::bind_method(D_METHOD("getBodyJointPositions2D", "bodyId"), &godot::Kodot2::getBodyJointPositions2D);

    // -- Get/set --
    ClassDB::bind_method(D_METHOD("getBodyCount"), &godot::Kodot2::getBodyCount);

    // -- Exports --
    ClassDB::bind_method(D_METHOD("get_printVerboseErrors"), &godot::Kodot2::get_printVerboseErrors);
	ClassDB::bind_method(D_METHOD("set_printVerboseErrors", "p_printVerboseErrors"), &godot::Kodot2::set_printVerboseErrors);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "printVerboseErrors"), "set_printVerboseErrors", "get_printVerboseErrors");

    ClassDB::bind_method(D_METHOD("get_screenSize"), &godot::Kodot2::get_screenSize);
	ClassDB::bind_method(D_METHOD("set_screenSize", "p_screenSize"), &godot::Kodot2::set_screenSize);
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

    // Setup bodies


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

godot::TypedArray<godot::Vector2> godot::Kodot2::update(double delta)
{
    TypedArray<Vector2> jointPoints;

    if (!bodyFrameReader)
    {
        return jointPoints;
    }

    IBodyFrame* bodyFrame = NULL;
    HRESULT hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

    if (SUCCEEDED(hr))
    {
        // NOTE: I'm not sure if we really need the timestamp stuff
        // might remove this later
        INT64 time = 0;
        hr = bodyFrame->get_RelativeTime(&time);

        // Make sure to initialize memory with default value
        IBody* iBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            // Seems like _countof is just a constant value...'
            // Might just replace with BODY_COUNT later
            hr = bodyFrame->GetAndRefreshBodyData(_countof(iBodies), iBodies);
        }

        if (SUCCEEDED(hr))
        {
            jointPoints = processBody(time, BODY_COUNT, iBodies);
        }

        // Make sure to free the iBodies
        for (int i = 0; i < _countof(iBodies); i++)
        {
            SafeRelease(iBodies[i]);
        }
    }
    SafeRelease(bodyFrame);
    return jointPoints;
}

godot::TypedArray<godot::Vector2> godot::Kodot2::processBody(uint64_t nTime, int bodyCount, IBody** iBodies)
{
    HRESULT hr;
    TypedArray<Vector2> jointPoints;
    for (int i = 0; i < bodyCount; i++)
    {
        IBody* body = iBodies[i];
        if (body)
        {
            BOOLEAN tracked = false;
            hr = body->get_IsTracked(&tracked);
            
            if (SUCCEEDED(hr) && tracked)
            {
                Joint joints[JointType_Count];
                HandState leftHandState = HandState_Unknown;
                HandState rightHandState = HandState_Unknown;

                body->get_HandLeftState(&leftHandState);
                body->get_HandRightState(&rightHandState);

                hr = body->GetJoints(_countof(joints), joints);
                if (SUCCEEDED(hr))
                {
                    for (int j = 0; j < _countof(joints); j++)
                    {
                        // Currently: Stop at the first body that we find
                        CameraSpacePoint jPos = joints[j].Position;
                        jointPoints.push_back(bodyToScreen(Vector3(
                            jPos.X, jPos.Y, jPos.Z
                        )));

                        // jointPoints.push_back(Vector3(
                        //     jPos.X, jPos.Y, jPos.Z
                        // ));
                    }
                    return jointPoints;
                }
            }
        }
    }
    print_line("Searching for bodies...");
    return jointPoints;
}

bool godot::Kodot2::getJoints(int bodyId, _Joint* joints)
{
    // if (bodyId < 0 || bodyId >= BODY_COUNT)
    // {
    //     print_error("Error: bodyId is out of range");
    //     return true;
    // }

    // IBody* body;
    // bool bodyFound = false;
    // for (int i = 0; i < BODY_COUNT; i++)
    // {
    //     body = bodies[i];
    //     if (body)
    //     {
    //         bodyFound = true;
    //         break;
    //     }
    // }

    // if (!bodyFound)
    // {
    //     print_error("Error: No valid body found.");
    //     return true;
    // }

    // BOOLEAN tracked = false;
    // HRESULT hr = body->get_IsTracked(&tracked);

    // // if (!(SUCCEEDED(hr) && tracked))
    // // {
    // //     print_error("Error: Body is not tracked.");
    // //     return true;
    // // }

    // if (!SUCCEEDED(hr))
    // {
    //     print_error("Body didn't succeed");
    //     return true;
    // }

    // if (!tracked)
    // {
    //     // print_error("Body isn't tracked");
    //     return true;
    // }

    // hr = body->GetJoints(JointType_Count, joints);
    // if (!SUCCEEDED(hr))
    // {
    //     print_error("Error: Unable to get joints");
    //     return true;
    // }

    return false;
}

godot::TypedArray<godot::Vector2> godot::Kodot2::getBodyJointPositions2D(int bodyId)
{
    TypedArray<Vector2> jointPoints;
    Joint joints[JointType_Count];

    if (getJoints(bodyId, joints))
    {
        // Failed to get joints
        return jointPoints;
    }

    for (int i = 0; i < _countof(joints); i++)
    {
        // Convert points to screen-space
        CameraSpacePoint jPos = joints[i].Position;
        jointPoints.push_back(bodyToScreen(jPos.X, jPos.Y, jPos.Z));
    }

    return jointPoints;
}

godot::TypedDictionary<int, godot::Vector3> godot::Kodot2::getBodyJointPositions3D(int bodyId)
{
    TypedDictionary<int, Vector3> jointPoints;
    Joint joints[JointType_Count];

    if (getJoints(bodyId, joints))
    {
        // Failed to get joints
        return jointPoints;
    }

    for (int i = 0; i < JointType_Count; i++)
    {
        CameraSpacePoint jPos = joints[i].Position;
        // jointPoints.push_back(Vector3(jPos.X, jPos.Y, jPos.Z));
        jointPoints.get_or_add(i, Vector3(jPos.X, jPos.Y, jPos.Z));
    }

    return jointPoints;
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
int godot::Kodot2::getBodyCount()
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


// -- Debug --
void godot::Kodot2::printErr(const godot::Variant &p_variant)
{
    if (!printVerboseErrors)
    {
        return;
    }
    print_error(p_variant);
}
