#include "kodot2.h"

#include "util.h"
#include "windowsdef.h"
#include <Kinect.h>

#include <godot_cpp/core/class_db.hpp>


void godot::Kodot2::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot2::initialize);
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot2::update);

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
        INT64 nTime = 0;
        hr = bodyFrame->get_RelativeTime(&nTime);

        IBody* bodies[BODY_COUNT] = {0};

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
    print_line("Updated"); 
}

void godot::Kodot2::processBody(uint64_t nTime, int bodyCount, IBody** bodies)
{
    HRESULT hr;
    for (int i = 0; i < bodyCount; i++)
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

        hr = body->GetJoints(_countof(joints), joints);
        if (!SUCCEEDED(hr))
        {
            continue;
        }

        for (int j = 0; j < _countof(joints); j++)
        {
            // jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
        }
    }
}

// TODO: Add position type (2D/3D) OH i need two functions
void godot::Kodot2::getBodyJoints(int bodyId)
{

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
