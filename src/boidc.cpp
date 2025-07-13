#include "boidc.h"

#include "util.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/binder_common.hpp>

using namespace godot;


void BoidC::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_maxVelocity"), &BoidC::get_maxVelocity);
    ClassDB::bind_method(D_METHOD("set_maxVelocity", "p_maxVelocity"), &BoidC::set_maxVelocity);

    ClassDB::bind_method(D_METHOD("get_maxAcceleration"), &BoidC::get_maxVelocity);
    ClassDB::bind_method(D_METHOD("set_maxAcceleration", "p_maxAcceleration"), &BoidC::set_maxAcceleration);
}


BoidC::BoidC()
{}

BoidC::~BoidC()
{}

void BoidC::_ready()
{
    velocity = Vector2(fRandRange(-maxVelocity, maxVelocity), fRandRange(-maxVelocity, maxVelocity));
}

void BoidC::_process(double delta)
{
    velocity += acceleration.limit_length(maxAcceleration * delta);
	velocity = velocity.limit_length(maxVelocity);
	acceleration.x = 0;
	acceleration.y = 0;

    set_position(get_position() + velocity * delta);
    look_at(get_position() + velocity);
    set_rotation(get_rotation() + rotationOffset);
}

double BoidC::get_maxVelocity() const
{
    return maxVelocity;
}
void BoidC::set_maxVelocity(const double p_maxVelocity)
{
    maxVelocity = p_maxVelocity;
}

double BoidC::get_maxAcceleration() const
{
    return maxAcceleration;
}
void BoidC::set_maxAcceleration(const double p_maxAcceleration)
{
    maxAcceleration = p_maxAcceleration;
}

