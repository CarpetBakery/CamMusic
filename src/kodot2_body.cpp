#include "kodot2_body.h"
#include "kodot2.h"

using namespace godot;


void Kodot2Body::_bind_methods() 
{
    // ClassDB::bind_method(D_METHOD("getJointPositions3D"), &Kodot2Body::getJointPositions3D);
    ClassDB::bind_method(D_METHOD("get_joint_positions_3d"), &Kodot2Body::getJointPositions3D);
    ClassDB::bind_method(D_METHOD("get_joint_positions_2d"), &Kodot2Body::getJointPositions2D);

    ClassDB::bind_method(D_METHOD("isTracked"), &Kodot2Body::isTracked);
}


Kodot2Body::Kodot2Body() 
{
    // Allocate joints array
    joints.resize(JointType::Count);
    joints2D.resize(JointType::Count);
}

Kodot2Body::~Kodot2Body() 
{
}

TypedArray<Vector3> Kodot2Body::getJointPositions3D()
{
    return joints;
}

TypedArray<Vector2> Kodot2Body::getJointPositions2D()
{
    return joints2D;
}

bool Kodot2Body::isTracked()
{
    return tracked;
}
