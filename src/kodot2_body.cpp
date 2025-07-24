#include "kodot2_body.h"

using namespace godot;

void Kodot2Body::_bind_methods() 
{
    // -- Exports --
    ClassDB::bind_method(D_METHOD("get_isTracked"), &godot::Kodot2Body::get_isTracked);
	ClassDB::bind_method(D_METHOD("set_isTracked", "p_isTracked"), &godot::Kodot2Body::set_isTracked);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "isTracked"), "set_isTracked", "get_isTracked");

    ClassDB::bind_method(D_METHOD("get_joints"), &godot::Kodot2Body::get_joints);
	ClassDB::bind_method(D_METHOD("set_joints", "p_joints"), &godot::Kodot2Body::set_joints);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "joints"), "set_joints", "get_joints");
}


Kodot2Body::Kodot2Body() 
{
    print_line("Kodot Body Created");
}

Kodot2Body::~Kodot2Body() 
{
    print_line("Kodot Body Destroyed");
}


// -- Exports --
void Kodot2Body::set_isTracked(bool const p_isTracked)
{
    isTracked = p_isTracked;
}
bool Kodot2Body::get_isTracked() const
{
    return isTracked;
}

void Kodot2Body::set_joints(TypedArray<Vector3> const p_joints)
{
    joints = p_joints;
}
TypedArray<Vector3> Kodot2Body::get_joints() const
{
    return joints;
}
