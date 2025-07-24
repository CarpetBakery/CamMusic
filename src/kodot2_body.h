#ifndef KODOT2_BODY_H
#define KODOT2_BODY_H

#include <godot_cpp/classes/resource.hpp>


namespace godot
{
   class Kodot2Body : public Resource
    {
        GDCLASS(Kodot2Body, Resource)

    protected:
        static void _bind_methods();
    
    private:
        
    public:
        // Is this body currently being tracked?
        bool isTracked = false;

        // Joint positions in 3D Space
        TypedArray<Vector3> joints;

        Kodot2Body();
        ~Kodot2Body();

        String toString();

        // -- Exports --
        void set_isTracked(bool const p_isTracked);
        bool get_isTracked() const;

        void set_joints(TypedArray<Vector3> const p_joints);
        TypedArray<Vector3> get_joints() const;
    };
}


#endif // KODOT2_BODY_H