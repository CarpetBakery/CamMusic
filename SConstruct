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
env.Append(CPPPATH=["C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc/", "src/"])
sources = Glob("src/*.cpp")

# Add libraries
env.Append(LIBPATH = ["C:/Program Files/Microsoft SDKs/Kinect/v1.8/lib/"])
env.Append(LIBS = [
    "Kinect10"
])
# env.Append(INCPATH = ["C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc/"])

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