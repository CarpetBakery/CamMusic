#ifndef KODOT2_H
#define KODOT2_H

// For UINT64, USHORT, etc
// TODO: Replace this with standard types
#include "windowsdef.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

#include "kodot2_body.h"

// Forward declare Kinect types
// TODO: Do this for Kodot 1
class IKinectSensor;
class ICoordinateMapper;
class IBodyFrameReader;
class IDepthFrameReader;
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

    enum KodotHandState
    {
        Unknown	= 0,
        NotTracked	= 1,
        Open	= 2,
        Closed	= 3,
        Lasso	= 4
    };

    class Kodot2 : public Node
    {
        GDCLASS(Kodot2, Node);

    protected:
        static void _bind_methods();

    private:
        static const int DEPTH_WIDTH = 512;
        static const int DEPTH_HEIGHT = 424;

        // Was the Kinect initialized?
        bool isKinectInitialized = false;

        // The dimensions of the screen
        Vector2 screenSize = Vector2(1920, 1080);

        // Array of bodies that we can track
        TypedArray<Kodot2Body> kodotBodies;

        // TODO: Maybe remove this later it's kind of ambiguous
        bool printVerboseErrors = false;

        // How many bodies are currently being tracked?
        int trackedBodyCount = 0;

        // Which index was previously the first tracked body?
        int firstTrackedBodyIndex = 0;

        // The current depth image
        Ref<ImageTexture> depthTexture;


        void updateBody();
        void updateDepth();

        // Process all bodies in the current frame
        void processBodies(uint64_t nTime, int bodyCount, IBody** bodies);
        // Process depth from the current frame
        void processDepth(INT64 nTime, const UINT16* buffer, int width, int height, USHORT minDepth, USHORT maxDepth);

        // Convert world coord to screen
        Vector2 bodyToScreen(float x, float y, float z);
        Vector2 bodyToScreen(Vector3 bodyPoint);
        
        // Get either first tracked body, or body index
        Kodot2Body* getBodyFirstTrackedOrId(int bodyId = -1);

        // -- Debug --
        void printErr(const godot::Variant &p_variant);

    public:
        // Current Kinect
        IKinectSensor* kinectSensor;
        ICoordinateMapper* coordMapper;

        // Body reader
        IBodyFrameReader* bodyFrameReader;
        // Depth reader
        IDepthFrameReader* depthFrameReader;

        Kodot2();
        ~Kodot2();

        void _notification(int p_what);
        
        // Initialize Kinect sensor
        bool kinectInitialize();

        // Call at the beginning of the frame to update bodies and their joints
        void kinectUpdate();

        // -- Skeletal tracking --
        // Get joint positions in 3D space (default value gets the first tracked body)
        TypedArray<Vector3> getBodyJointPositions3D(int bodyId = -1);
        // Get joint positions in 2D space
        TypedArray<Vector2> getBodyJointPositions2D(int bodyId = -1);

        KodotHandState getBodyLeftHandState(int bodyId = -1);
        KodotHandState getBodyRightHandState(int bodyId = -1);

        // Get a body from the bodies array
        Kodot2Body* getBody(int _bodyIndex);
        
        // Return our kodotBodies array
        TypedArray<Kodot2Body> getAllBodies();

        // Get first tracked body in our bodies array
        Kodot2Body* getFirstTrackedBody();
        // The GDscript-facing function
        Ref<Kodot2Body> getFirstTrackedBodyExternal();

        // Get all tracked bodies in our bodies array
        TypedArray<Kodot2Body> getTrackedBodies();

        // Get the number of currently tracked bodies
        int getTrackedBodyCount();
        
        // -- Image --
        Ref<ImageTexture> getDepthTexture();
        Ref<ImageTexture> imageTest(int width, int height, Color color);

        // -- Exports --
        void set_printVerboseErrors(bool const p_printVerboseErrors);
        bool get_printVerboseErrors() const;

        void set_screenSize(Vector2 const p_screenSize);
        Vector2 get_screenSize() const;
    };
}

// Needed to expose enum to GDScript
VARIANT_ENUM_CAST(JointType);
VARIANT_ENUM_CAST(KodotHandState);


#endif // KODOT2_H