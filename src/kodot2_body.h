#ifndef KODOT2_BODY_H
#define KODOT2_BODY_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>


namespace godot
{
   class Kodot2Body : public Node
    {
        GDCLASS(Kodot2Body, Node)

    protected:
        static void _bind_methods();
    
    private:
        
    public:
        
    };
}


#endif // KODOT2_BODY_H