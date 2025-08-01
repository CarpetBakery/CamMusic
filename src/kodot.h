#ifndef KODOT_H
#define KODOT_H

#include "windowsdef.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>


struct Kinect
{
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
        void _ready() override;
        void _exit_tree() override;

        bool initialize();
        void update(double delta);
        TypedDictionary<int, Vector2> getSkeletonJoints(int skeletonId);
        TypedDictionary<int, Vector3> getSkeletonJoints3D();

        // -- Get/set --
        void set_sensorAngle(int p_sensorAngle);
        int get_sensorAngle();
        
        // -- Export --
        void set_seatedMode(bool const p_seatedMode);
        bool get_seatedMode() const;

        void set_screenSize(Vector2 const p_screenSize);
        Vector2 get_screenSize() const;
    };
}

#endif // KODOT_H