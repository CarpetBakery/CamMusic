#include "kodot.h"

#include <godot_cpp/core/class_db.hpp>
#include "NuiApi.h"
#include "NuiSkeleton.h"
#include "util.h"


struct NuiTypes
{
    typedef NUI_SKELETON_FRAME SkeletonFrame;
    typedef NUI_SKELETON_DATA SkeletonData;
};


void godot::Kodot::_bind_methods()
{
    // -- Functions --
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot::update);
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot::initialize);
    ClassDB::bind_method(D_METHOD("getSkeletonJoints", "skeletonId"), &godot::Kodot::getSkeletonJoints);

    // -- Get/set --
    ClassDB::bind_method(D_METHOD("set_sensorAngle", "p_sensorAngle"), &godot::Kodot::set_sensorAngle);
    ClassDB::bind_method(D_METHOD("get_sensorAngle"), &godot::Kodot::get_sensorAngle);
    
    // -- Exported vars --
    ClassDB::bind_method(D_METHOD("get_seatedMode"), &godot::Kodot::get_seatedMode);
	ClassDB::bind_method(D_METHOD("set_seatedMode", "p_seatedMode"), &godot::Kodot::set_seatedMode);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "seatedMode"), "set_seatedMode", "get_seatedMode");

    ClassDB::bind_method(D_METHOD("get_screenSize"), &godot::Kodot::get_screenSize);
	ClassDB::bind_method(D_METHOD("set_screenSize", "p_screenSize"), &godot::Kodot::set_screenSize);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "screenSize"), "set_screenSize", "get_screenSize");
}

void godot::Kodot::_ready() {}

void godot::Kodot::_exit_tree()
{
    if (kinect.sensor)
    {
        godot::print_line("Shutting down kinect...");
        kinect.sensor->Release();
        kinect.sensor->NuiShutdown();
    }
}


INuiSensor* _sensor; // Not great
bool godot::Kodot::initialize()
{
    int sensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&sensorCount);
    if (FAILED(hr))
    {
        godot::print_line("Error: Failed to initialize Kodot.");
        return true;
    }

    for (int i = 0; i < sensorCount; i++)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &_sensor);
        if (FAILED(hr))
        {
            godot::print_line("Error: Failed to create new sensor.");
            continue;
        }

        // Get status of sensor, and if connected, we can initialize it
        hr = _sensor->NuiStatus();
        if (S_OK == hr)
        {
            kinect.sensor = _sensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        _sensor->Release();
    }

    if (NULL != kinect.sensor)
    {
        hr = kinect.sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
        if (SUCCEEDED(hr))
        {
            // Create an event that will be signaled when skeleton data is available
            kinect.hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

            // Open a skeleton stream to receive skeleton data
            // NOTE: Also enable seated mode
            hr = kinect.sensor->NuiSkeletonTrackingEnable(kinect.hNextSkeletonEvent, seatedMode ? NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT : NULL); 
        }
    }

    if (NULL == kinect.sensor || FAILED(hr))
    {
        godot::print_error("Error: No ready Kinect found.");
        return true;
    }
    
    godot::print_line("Initialized Kodot.");
    godot::print_line(sensorCount);
    return false;
}

void godot::Kodot::update(double delta)
{
    
}

// CURRENTLY: Get the first skeleton that we see and return its joints
// as a bunch of 2D positions
godot::TypedDictionary<int, godot::Vector2> godot::Kodot::getSkeletonJoints(int skeletonId = 0)
{
    godot::TypedDictionary<int, godot::Vector2> joints;
    
    // Don't try to get an invalid skeleton ID
    if (skeletonId > NUI_SKELETON_COUNT - 1 || skeletonId < 0)
    {
        godot::print_error("Error: SkeletonId out of range. (" + godot::String(std::to_string(skeletonId).c_str()) + ")");
        return joints;
    }
    
    // Try to get the skeletons from our sensor
    NuiTypes::SkeletonFrame skeletonFrame;
    HRESULT hr = kinect.sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
    if (FAILED(hr))
    {
        // trace("Error: Failed to get the damn skeleton frame.");
        return joints;
    }

    // Smooth out skeleton data
    {
        NUI_TRANSFORM_SMOOTH_PARAMETERS params = {0.5f, 0.5f, 0.5f, 0.05f, 0.04f};
        kinect.sensor->NuiTransformSmooth(&skeletonFrame, NULL);
    }

    // See if we can find a valid skeleton in this frame
    bool foundSkeleton = false;
    NUI_SKELETON_DATA skeletonData;
    for (int i = 0; i < NUI_SKELETON_COUNT; i++)
    {
        // OLD
        // NUI_SKELETON_DATA &skeletonData = skeletonFrame.SkeletonData[skeletonId]; 

        skeletonData = skeletonFrame.SkeletonData[i]; 
        if (NUI_SKELETON_TRACKED == skeletonData.eTrackingState)
        {
            // We're tracking the skeleton
            foundSkeleton = true;
            break;
        }
    }
    if (!foundSkeleton)
    {
        return joints;
    }

    // Put all joint positions into the array
    // TODO: Remove these temp vars
    LONG x, y;
    USHORT depth;
    int jointCount = 0;
    for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
    {
        // Check joint state
        NUI_SKELETON_POSITION_TRACKING_STATE jointState = skeletonData.eSkeletonPositionTrackingState[i];
        if (jointState != NUI_SKELETON_POSITION_TRACKED)
        {
            continue;
        }

        NuiTransformSkeletonToDepthImage(skeletonData.SkeletonPositions[i], &x, &y, &depth);
        godot::Vector2 jointPoint = godot::Vector2(
            static_cast<float>(x), 
            static_cast<float>(y)
        );

        joints.get_or_add(i, jointPoint);
        jointCount += 1;
    }
    // godot::print_line("Getting " + godot::String(std::to_string(jointCount).c_str()) + " joints...");
    return joints;
}


// -- Get/set --
void godot::Kodot::set_sensorAngle(int p_sensorAngle)
{
    if (p_sensorAngle > 27 || p_sensorAngle < -27)
    {
        godot::print_error("Error: Trying to set Kinect angle out of range (+/-27).");
        return;
    }

    godot::print_line_rich("[color=yellow]KINECT ANGLE CHANGE TO: " + godot::String(std::to_string(p_sensorAngle).c_str()) + " DEGREES.");
    kinect.sensor->NuiCameraElevationSetAngle(p_sensorAngle);
} 
int godot::Kodot::get_sensorAngle()
{
    long angle;
    kinect.sensor->NuiCameraElevationGetAngle(&angle);

    return static_cast<int>(angle);
}


// -- Exports --
void godot::Kodot::set_seatedMode(bool const p_seatedMode)
{
    seatedMode = p_seatedMode;
}
bool godot::Kodot::get_seatedMode() const
{
    return seatedMode;
}

void godot::Kodot::set_screenSize(godot::Vector2 const p_screenSize)
{
    screenSize = p_screenSize;
}
godot::Vector2 godot::Kodot::get_screenSize() const
{
    return screenSize;
}