#ifndef KODOT2_H
#define KODOT2_H

#include <godot_cpp/classes/node.hpp>

// Forward declare Kinect types
// TODO: Do this for Kodot 1
class IKinectSensor;
class ICoordinateMapper;
class IBodyFrameReader;
class IBody;

namespace godot
{
    class Kodot2 : public Node
    {
        GDCLASS(Kodot2, Node);

    private:
        static const int DEPTH_WIDTH = 512;
        static const int DEPTH_HEIGHT = 424;

        // TODO: Maybe remove this later it's kind of ambiguous
        bool printVerboseErrors = false;

        // Number of currently detected bodies
        int bodyCount = 0;

        // Acquire the next body frame
        void processBody(uint64_t nTime, int bodyCount, IBody** bodies);
        

        // -- Debug --
        void printErr(const godot::Variant &p_variant);

    protected:
        static void _bind_methods();

    public:
        // Current Kinet
        IKinectSensor* kinectSensor;
        ICoordinateMapper* coordMapper;

        // Body reader
        IBodyFrameReader* bodyFrameReader;

        // Initialize Kinect sensor
        bool initialize();

        // NOTE: Not sure if I'll use this one
        void update(double delta);

        // Get an array of joint positions based on bodyId
        void getBodyJoints(int bodyId);

        
        // -- Exports --
        void set_printVerboseErrors(bool const p_printVerboseErrors);
        bool get_printVerboseErrors() const;


        // -- Get/set --
        // Get the number of currently detected bodies
        int getBodyCount();        

    };
}

#endif // KODOT2_H