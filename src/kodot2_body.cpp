#include "kodot2_body.h"
#include "kodot2.h"

#include <godot_cpp/classes/engine.hpp>

using namespace godot;


void Kodot2Body::_bind_methods() 
{
    ClassDB::bind_method(D_METHOD("isTracked"), &Kodot2Body::isTracked);
    ClassDB::bind_method(D_METHOD("get_joint_positions_3d"), &Kodot2Body::getJointPositions3D);
    ClassDB::bind_method(D_METHOD("get_joint_positions_2d"), &Kodot2Body::getJointPositions2D);
    ClassDB::bind_method(D_METHOD("get_left_hand_state"), &Kodot2Body::getLeftHandState);
    ClassDB::bind_method(D_METHOD("get_right_hand_state"), &Kodot2Body::getRightHandState);
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

int Kodot2Body::getLeftHandState()
{
    return leftHandState;
}
int Kodot2Body::getRightHandState()
{
    return rightHandState;
}


Kodot2Body::Kodot2Body() 
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    leftHandState = 0;
    rightHandState = 0;

    // Allocate joints array
    joints.resize(JointType::Count);
    joints2D.resize(JointType::Count);
}

Kodot2Body::~Kodot2Body() 
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
}