#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <string>

double fRandRange(double min, double max);
void trace(std::string str, bool includeTimestamp = true);


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