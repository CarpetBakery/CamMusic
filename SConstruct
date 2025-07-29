#!/usr/bin/env python
import os
import sys

GODOT_PROJECT_NAME = "CamMusic"
EXTENSION_NAME = "kodot"


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


# Include documentation
if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
        print("Including documentation...")
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")


if env["platform"] == "macos":
    library = env.SharedLibrary(
        GODOT_PROJECT_NAME + "/bin/" + EXTENSION_NAME + ".{}.{}.framework/" + EXTENSION_NAME + ".{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            GODOT_PROJECT_NAME + "/bin/" + EXTENSION_NAME + ".{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            GODOT_PROJECT_NAME + "/bin/" + EXTENSION_NAME + ".{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        GODOT_PROJECT_NAME + "/bin/" + EXTENSION_NAME + "{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )


Default(library)