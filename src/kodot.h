#ifndef KODOT_H
#define KODOT_H

#include <godot_cpp/classes/node3d.hpp>

struct Kinect
{
public:
    // Current connected Kinect sensor
    class INuiSensor* sensor;
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
        Kodot() = default;
    
        void _ready() override;

        bool initialize();
        void update(double delta);
    };
}

#endif // KODOT_H