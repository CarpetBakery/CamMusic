#include "kodot2.h"

#include "util.h"
#include "windowsdef.h"
#include <Kinect.h>

#include <godot_cpp/core/class_db.hpp>


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
}


bool godot::Kodot2::initialize()
{
    print_line("Initializing Kodot 2");

    HRESULT hr;
    hr = GetDefaultKinectSensor(&kinectSensor);
    if (FAILED(hr))
    {
        godot::print_error("Error: Failed to get default sensor.");
        return true;
    }

    if (kinectSensor)
    {
        // Initialize Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* bodyFrameSource = NULL;

        hr = kinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = kinectSensor->get_CoordinateMapper(&coordMapper);
        }

        if (SUCCEEDED(hr))
        {
            hr = kinectSensor->get_BodyFrameSource(&bodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = bodyFrameSource->OpenReader(&bodyFrameReader);
        }

        SafeRelease(bodyFrameSource);
    }

    if (!kinectSensor || FAILED(hr))
    {
        godot::print_error("Error: No ready Kinect found.");
        return true;
    }

    print_line("Kodot 2 initialized.");
    return false;
}

void godot::Kodot2::_exit_tree()
{
    // Shutdown stuff here
    // TODO: 
    SafeRelease(bodyFrameReader);
    SafeRelease(coordMapper);

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
        print_error("No body frame reader");
        return; 
    }

    IBodyFrame* bodyFrame = NULL;
    HRESULT hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;
        hr = bodyFrame->get_RelativeTime(&nTime);

        // IBody* bodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
        }

        if (SUCCEEDED(hr))
        {
            // Update skeleton
            processBody(nTime, BODY_COUNT, bodies);
        }
    }
    SafeRelease(bodyFrame);

    // Figure out how many bodies are in this frame
    // TODO:
}

void godot::Kodot2::processBody(uint64_t nTime, int _bodyCount, IBody** bodies)
{
    HRESULT hr;
    for (int i = 0; i < _bodyCount; i++)
    {
        IBody* body = bodies[i];
        if (!body)
        {
            continue;
        }

        BOOLEAN tracked = false;
        hr = body->get_IsTracked(&tracked);

        if (!(SUCCEEDED(hr) && tracked))
        {
            continue;
        }

        Joint joints[JointType_Count];
        Vector3 jointPoints[JointType_Count];
        HandState leftHandState = HandState_Unknown;
        HandState rightHandState = HandState_Unknown;

        body->get_HandLeftState(&leftHandState);
        body->get_HandRightState(&rightHandState);

        // hr = body->GetJoints(_countof(joints), joints);
        hr = body->GetJoints(JointType_Count, joints);
        if (!SUCCEEDED(hr))
        {
            continue;
        }

        // TypedArray<Vector2> test;
        // // for (int j = 0; j < _countof(joints); j++)
        // for (int j = 0; j < JointType_Count; j++)
        // {
        //     // jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
        //     CameraSpacePoint jPos = joints[j].Position;
        //     test.push_back(bodyToScreen(jPos.X, jPos.Y ,jPos.Z));
        //     // test.push_back(Vector2(2, 2));
        // }
    }
}

bool godot::Kodot2::getJoints(int bodyId, _Joint* joints)
{
    if (bodyId < 0 || bodyId >= bodyCount)
    {
        print_error("Error: bodyId is out of range");
        return true;
    }

    // IBody* body = bodies[bodyId];
    IBody* body;
    bool bodyFound = false;
    for (int i = 0; i < bodyCount; i++)
    {
        body = bodies[i];
        if (body)
        {
            bodyFound = true;
            break;
        }
    }

    if (!bodyFound)
    {
        print_error("Error: No valid body found.");
        return true;
    }

    BOOLEAN tracked = false;
    HRESULT hr = body->get_IsTracked(&tracked);

    // if (!(SUCCEEDED(hr) && tracked))
    // {
    //     print_error("Error: Body is not tracked.");
    //     return true;
    // }

    if (!SUCCEEDED(hr))
    {
        print_error("Body didn't succeed");
        return true;
    }

    if (!tracked)
    {
        // print_error("Body isn't tracked");
        return true;
    }

    hr = body->GetJoints(JointType_Count, joints);
    if (!SUCCEEDED(hr))
    {
        print_error("Error: Unable to get joints");
        return true;
    }

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


// -- Exports --
void godot::Kodot2::set_printVerboseErrors(bool const p_printVerboseErrors)
{
    printVerboseErrors = p_printVerboseErrors;
}
bool godot::Kodot2::get_printVerboseErrors() const
{
    return printVerboseErrors;
}


// -- Get/set --
int godot::Kodot2::getBodyCount()
{
    return bodyCount;
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
