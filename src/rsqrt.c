#include <stddef.h>
#include <stdint.h>

union convert { float f; int32_t i; };

const float threeHalfs = 1.5F;
const int32_t magic = 0x5f3759df;

float Q_rsqrt(float number)
{	
	const float x2 = number * 0.5F;
	union convert converter = {number};

	converter.i = magic - (converter.i >> 1);
	converter.f *= threeHalfs - x2 * converter.f * converter.f;

	return converter.f;
}


void Q_rsqrt_vec(size_t size, float* array) {
	for (size_t i = 0; i < size; i++) {
		array[i] = Q_rsqrt(array[i]);
	}
}
