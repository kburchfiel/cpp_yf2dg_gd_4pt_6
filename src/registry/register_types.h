// Source: https://docs.godotengine.org/en/4.5/tutorials/scripting/cpp/gdextension_cpp_example.html#id4

#pragma once

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level);
void uninitialize_example_module(ModuleInitializationLevel p_level);