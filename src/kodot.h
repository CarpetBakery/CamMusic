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
    };
}

#endif // KODOT_H