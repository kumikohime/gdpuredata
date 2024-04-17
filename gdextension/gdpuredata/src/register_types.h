#ifndef GDPUREDATA_REGISTER_TYPES_H
#define GDPUREDATA_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_puredata_module(ModuleInitializationLevel p_level);
void uninitialize_puredata_module(ModuleInitializationLevel p_level);

#endif // GDPUREDATA_REGISTER_TYPES_H