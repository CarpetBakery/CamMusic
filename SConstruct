#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=[
    "src/",

    "C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc/", 
    "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc/",
])
sources = Glob("src/*.cpp")

# Add libraries
env.Append(LIBPATH = [
    "C:/Program Files/Microsoft SDKs/Kinect/v1.8/lib/amd64/",
    "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/",
])
env.Append(LIBS = [
    "Kinect10",
    "Kinect20",
])


godotProjectName = "CamMusic"
extensionName = "kodot"

if env["platform"] == "macos":
    library = env.SharedLibrary(
        godotProjectName + "/bin/" + extensionName + ".{}.{}.framework/" + extensionName + ".{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            godotProjectName + "/bin/" + extensionName + ".{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            godotProjectName + "/bin/" + extensionName + ".{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        godotProjectName + "/bin/" + extensionName + "{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)