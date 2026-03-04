// Source: https://docs.godotengine.org/en/4.5/tutorials/scripting/cpp/gdextension_cpp_example.html

#include "register_types.h"

#include "entity/player.h"
#include "entity/mob.h"
#include "scene/main.h"
#include "scene/hud.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Changed the following from GDREGISTER_RUNTIME_CLASS (as found in
	// the Godot 4.5 GDExtension demo)
	// to GDREGISTER_CLASS.
	// For reference, see
	// https://docs.godotengine.org/en/4.5/tutorials/scripting/cpp/gdextension_cpp_example.html
	// and 
	// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html .
	// This *seemed* to be the only difference between these
	// two versions' register_types.cpp files.
	GDREGISTER_CLASS(Player);
	GDREGISTER_CLASS(Mob);
	GDREGISTER_CLASS(Main);
	GDREGISTER_CLASS(Hud);

}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(
GDExtensionInterfaceGetProcAddress p_get_proc_address, 
const GDExtensionClassLibraryPtr p_library, 
GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(
        p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}