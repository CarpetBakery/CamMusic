#ifndef KODOT_H
#define KODOT_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>
#include <windows.h>

#define WIN32_LEAN_AND_MEAN


struct Kinect
{
public:
    // Current connected Kinect sensor
    class INuiSensor* sensor;
    HANDLE hNextSkeletonEvent;
};

namespace godot
{
    class Kodot : public Node3D
    {
        GDCLASS(Kodot, Node3D);

    private:
        Kinect kinect;

        bool seatedMode = false;
        Vector2 screenSize = Vector2(1920, 1080);
        
    protected:
        static void _bind_methods();

    public:
        // enum SKELETON_POSITION_INDEX
        // {
        //     HIP_CENTER = 0,
        //     SPINE,
        //     SHOULDER_CENTER,
        //     HEAD,
        //     SHOULDER_LEFT,
        //     ELBOW_LEFT,
        //     WRIST_LEFT,
        //     HAND_LEFT,
        //     SHOULDER_RIGHT,
        //     ELBOW_RIGHT,
        //     WRIST_RIGHT,
        //     HAND_RIGHT,
        //     HIP_LEFT,
        //     KNEE_LEFT,
        //     ANKLE_LEFT,
        //     FOOT_LEFT,
        //     HIP_RIGHT,
        //     KNEE_RIGHT,
        //     ANKLE_RIGHT,
        //     FOOT_RIGHT,
        //     COUNT
        // };

        void _ready() override;
        void _exit_tree() override;

        bool initialize();
        void update(double delta);
        void processSkeleton(double delta);
        TypedDictionary<int, Vector2> getSkeletonJoints(int skeletonId);


        // -- Get/set --
        void set_seatedMode(bool const p_seatedMode);
        bool get_seatedMode() const;

        void set_screenSize(Vector2 const p_screenSize);
        Vector2 get_screenSize() const;
    };
}

#endif // KODOT_H