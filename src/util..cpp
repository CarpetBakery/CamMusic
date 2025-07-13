#include "util.h"

double fRandRange(double min, double max)
{
	double denom = static_cast<double>((double)RAND_MAX / (max - min));

	// Avoid dividing by zero
	if (denom == 0.0f)
		return 0;

	return min + (static_cast<double>(rand()) / denom);
}