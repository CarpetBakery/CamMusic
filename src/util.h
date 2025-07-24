#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <string>
#include <godot_cpp/variant/string.hpp>

double fRandRange(double min, double max);

// Debug print
void trace(std::string str, bool includeTimestamp = true);

// Get milliseconds in string form
godot::String getMsec();


// Safe release for interfaces (Used by Kinect2 example code)
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}

#endif //UTIL_H