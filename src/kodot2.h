#ifndef KODOT2_H
#define KODOT2_H

#include "windowsdef.h"

#include <godot_cpp/classes/node.hpp>

namespace godot
{
    class Kodot2 : public Node
    {
        GDCLASS(Kodot2, Node);

    private:

    protected:
    static void _bind_methods();

    public:
        bool initialize();
    };
}

#endif // KODOT2_H