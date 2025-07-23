#ifndef KODOT2_H
#define KODOT2_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

// Forward declare Kinect types
// TODO: Do this for Kodot 1
class IKinectSensor;
class ICoordinateMapper;
class IBodyFrameReader;
class IBody;

struct _Joint;


namespace godot
{
    class Kodot2 : public Node
    {
        GDCLASS(Kodot2, Node);

    private:
        static const int DEPTH_WIDTH = 512;
        static const int DEPTH_HEIGHT = 424;

        // The dimensions of the screen
        Vector2 screenSize = Vector2(1920, 1080);

        // Array of currently tracked bodies
        // NOTE: BODY_COUNT is defined as 6 in Kinect.h
        IBody* bodies[6] = {0};

        // Number of currently detected bodies
        // TODO: currently doesn't track properly. Always set to 6
        int bodyCount = 6;

        // TODO: Maybe remove this later it's kind of ambiguous
        bool printVerboseErrors = false;

        // UNUSED
        void processBody(uint64_t nTime, int _bodyCount, IBody** bodies);

        // Get a body's joints
        bool getJoints(int bodyId, _Joint* joints);

        // Convert world coord to screen
        Vector2 bodyToScreen(float x, float y, float z);
        Vector2 bodyToScreen(Vector3 bodyPoint);
        

        // -- Debug --
        void printErr(const godot::Variant &p_variant);

    protected:
        static void _bind_methods();

    public:
        // Current Kinect
        IKinectSensor* kinectSensor;
        ICoordinateMapper* coordMapper;

        // Body reader
        IBodyFrameReader* bodyFrameReader;

        // Initialize Kinect sensor
        bool initialize();
        
        void _exit_tree() override;

        // Call at the beginning of the frame to update bodies and their joints
        void update(double delta);

        // Get an array of joint positions based on bodyId
        TypedDictionary<int, Vector3> getBodyJointPositions3D(int bodyId);
        TypedArray<Vector2> getBodyJointPositions2D(int bodyId);


        // -- Exports --
        void set_printVerboseErrors(bool const p_printVerboseErrors);
        bool get_printVerboseErrors() const;


        // -- Get/set --
        // Get the number of currently detected bodies
        int getBodyCount();        

    };
}

#endif // KODOT2_H