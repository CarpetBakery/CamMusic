#include "kodot.h"

#include <godot_cpp/core/class_db.hpp>
#include "NuiApi.h"


void godot::Kodot::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot::update);
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot::initialize);
}

godot::Kodot::~Kodot()
{
    // Close sensor and handle if needed
    // TODO: Check if C++ nodes are constructed/destructed only once...
    if (kinect.sensor)
    {
        kinect.sensor->NuiShutdown();
    }

    if (kinect.hNextSkeletonEvent && (kinect.hNextSkeletonEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(kinect.hNextSkeletonEvent);
    }
}

void godot::Kodot::_ready() {}

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
            // godot::print_line("Error: Failed to create new sensor.");
            // return true;
            continue;
        }

        // Get status of sensor, and if connected, we ccan initialize it
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
            hr = kinect.sensor->NuiSkeletonTrackingEnable(kinect.hNextSkeletonEvent, 0); 
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