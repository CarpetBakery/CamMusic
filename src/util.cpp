#include "util.h"

#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/print_string.hpp>

double fRandRange(double min, double max)
{
	double denom = static_cast<double>((double)RAND_MAX / (max - min));

	// Avoid dividing by zero
	if (denom == 0.0f)
		return 0;

	return min + (static_cast<double>(rand()) / denom);
}

void trace(std::string str, bool includeTimestamp)
{
	godot::String timeStr = "";
	if (includeTimestamp)
	{
		std::string time = std::to_string(godot::Time::get_singleton()->get_ticks_msec());
		timeStr = godot::String(std::string(time).c_str());
	}
	godot::print_line(godot::String(str.c_str()) + timeStr);
}