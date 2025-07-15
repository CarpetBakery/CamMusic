#include "kodot.h"

#include <godot_cpp/core/class_db.hpp>
#include <windows.h>
#include "NuiApi.h"

#define WIN32_LEAN_AND_MEAN


void godot::Kodot::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update", "delta"), &godot::Kodot::update);
    ClassDB::bind_method(D_METHOD("initialize"), &godot::Kodot::initialize);
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
            godot::print_line("Error: Failed to create new sensor.");
            return true;
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
    
    godot::print_line("Initialized Kodot.");
    return false;
}

void godot::Kodot::update(double delta)
{

}