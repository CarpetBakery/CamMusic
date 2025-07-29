#include "kodot2_body.h"
#include "kodot2.h"

using namespace godot;


void Kodot2Body::_bind_methods() 
{
    ClassDB::bind_method(D_METHOD("getJointPositions3D"), &Kodot2Body::getJointPositions3D);
    ClassDB::bind_method(D_METHOD("getJointPositions2D"), &Kodot2Body::getJointPositions2D);

    // -- Exports --
    ClassDB::bind_method(D_METHOD("get_isTracked"), &Kodot2Body::get_isTracked);
	ClassDB::bind_method(D_METHOD("set_isTracked", "p_isTracked"), &Kodot2Body::set_isTracked);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "isTracked"), "set_isTracked", "get_isTracked");

    ClassDB::bind_method(D_METHOD("get_joints"), &Kodot2Body::get_joints);
	ClassDB::bind_method(D_METHOD("set_joints", "p_joints"), &Kodot2Body::set_joints);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "joints"), "set_joints", "get_joints");
}


Kodot2Body::Kodot2Body() 
{
    print_line("Kodot Body Created");
    
    // Allocate joints array
    joints.resize(JointType::Count);
    joints2D.resize(JointType::Count);
}

Kodot2Body::~Kodot2Body() 
{
    print_line("Kodot Body Destroyed");
}

String Kodot2Body::toString()
{
    if (isTracked)
    {
        return "true";
    }
    return "false";
}

TypedArray<Vector3> Kodot2Body::getJointPositions3D()
{
    return joints;
}

TypedArray<Vector2> Kodot2Body::getJointPositions2D()
{
    return joints2D;
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
