#ifndef WINDOWS_DEF_H
#define WINDOWS_DEF_H

// Includes the headers we need from windows.h to use Kinect APIs

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

// Seems to be excluded by lean and mean, but we need it for Kinects
#include <ShlObj.h>


#endif // WINDOWS_DEF_H