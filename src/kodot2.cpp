#include "kodot2.h"


#include <godot_cpp/core/class_db.hpp>


using namespace godot;


void Kodot2::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("initialize"), &Kodot2::initialize);
}


bool Kodot2::initialize()
{
    print_line("Kodot 2 initialized.");
    return false;
}