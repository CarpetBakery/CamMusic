#ifndef KODOT2_H
#define KODOT2_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

#include "kodot2_body.h"

// Forward declare Kinect types
// TODO: Do this for Kodot 1
class IKinectSensor;
class ICoordinateMapper;
class IBodyFrameReader;
class IBody;
struct _Joint;


namespace godot
{
    enum JointType
    {
        SpineBase		= 0,
        SpineMid		= 1,
        Neck			= 2,
        Head			= 3,
        ShoulderLeft	= 4,
        ElbowLeft		= 5,
        WristLeft		= 6,
        HandLeft		= 7,
        ShoulderRight	= 8,
        ElbowRight		= 9,
        WristRight		= 10,
        HandRight		= 11,
        HipLeft			= 12,
        KneeLeft		= 13,
        AnkleLeft		= 14,
        FootLeft		= 15,
        HipRight		= 16,
        KneeRight		= 17,
        AnkleRight		= 18,
        FootRight		= 19,
        SpineShoulder	= 20,
        HandTipLeft		= 21,
        ThumbLeft		= 22,
        HandTipRight	= 23,
        ThumbRight		= 24,
        Count			= ( ThumbRight + 1 ) 
    };

    class Kodot2 : public Node
    {
        GDCLASS(Kodot2, Node);

    protected:
        static void _bind_methods();

    private:
        static const int DEPTH_WIDTH = 512;
        static const int DEPTH_HEIGHT = 424;

        // The dimensions of the screen
        Vector2 screenSize = Vector2(1920, 1080);

        // TODO: Maybe remove this later it's kind of ambiguous
        bool printVerboseErrors = false;

        // UNUSED
        TypedArray<Vector2> processBody(uint64_t nTime, int bodyCount, IBody** bodies);

        // Get a body's joints
        bool getJoints(int bodyId, _Joint* joints);

        // Convert world coord to screen
        Vector2 bodyToScreen(float x, float y, float z);
        Vector2 bodyToScreen(Vector3 bodyPoint);
        

        // -- Debug --
        void printErr(const godot::Variant &p_variant);

    public:
        // Current Kinect
        IKinectSensor* kinectSensor;
        ICoordinateMapper* coordMapper;

        // Body reader
        IBodyFrameReader* bodyFrameReader;

        void _exit_tree() override;
        
        // Initialize Kinect sensor
        bool initialize();

        // Call at the beginning of the frame to update bodies and their joints
        TypedArray<Vector2> update(double delta);

        // Get an array of joint positions based on bodyId
        // TODO: Make these work
        TypedDictionary<int, Vector3> getBodyJointPositions3D(int bodyId);
        TypedArray<Vector2> getBodyJointPositions2D(int bodyId);


        // -- Exports --
        void set_printVerboseErrors(bool const p_printVerboseErrors);
        bool get_printVerboseErrors() const;

        void set_screenSize(Vector2 const p_screenSize);
        Vector2 get_screenSize() const;


        // -- Get/set --
        // Get the number of currently detected bodies
        int getBodyCount();        

    };
}

// Needed to expose enum to GDScript
VARIANT_ENUM_CAST(JointType)

#endif // KODOT2_H