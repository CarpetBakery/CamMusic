// Tells Godot about our extension

#include "register_types.h"

#include "gdexample.h"
#include "kodot.h"
#include "kodot2.h"
#include "kodot2_body.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) 
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) 
	{
		return;
	}

	// -- Register classes here --
	GDREGISTER_RUNTIME_CLASS(GDExample);
	GDREGISTER_RUNTIME_CLASS(Kodot);
	GDREGISTER_RUNTIME_CLASS(Kodot2);
	GDREGISTER_RUNTIME_CLASS(Kodot2Body);
}

void uninitialize_example_module(ModuleInitializationLevel p_level) 
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) 
	{
		return;
	}
}

extern "C" 
{
	// Initialization
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) 
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}