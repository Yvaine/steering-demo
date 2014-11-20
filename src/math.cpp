
#include "vmath.h"
#include <stdlib.h>

float randomReal(float max) {
	return max * (float(rand()) / RAND_MAX);
}

float randomBinomial(float max) {
	return randomReal(max)-randomReal(max);
}