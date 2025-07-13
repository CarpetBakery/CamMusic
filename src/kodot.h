#ifndef KODOT_H
#define KODOT_H

#include <godot_cpp/classes/node3d.hpp>
#include "NuiApi.h"


namespace godot
{
    class Kodot : public Node3D
    {
        GDCLASS(Kodot, Node3D);

    private:

    protected:
        static void _bind_methods();

    public:
        Kodot() = default;
    
        void _ready() override;  
    };
}

#endif // KODOT_H