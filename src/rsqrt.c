union convert { float f; unsigned int i; };

const float threeHalfs = 1.5F;
const unsigned int magic = 0x5f3759df;

float Q_rsqrt(float number)
{	
	const float x2 = number * 0.5F;
	union convert converter = {number};

	converter.i = magic - (converter.i >> 1);
	converter.f *= threeHalfs - x2 * converter.f * converter.f;

	return converter.f;
}

