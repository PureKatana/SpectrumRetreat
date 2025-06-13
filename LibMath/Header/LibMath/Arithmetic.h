#ifndef LIBMATH_ARITHMETIC_H_
#define LIBMATH_ARITHMETIC_H_

namespace LibMath
{
	bool almostEqual(float a, float b);				// Return if two floating value are similar enought to be considered equal

	float ceiling(float value);						// Return lowest integer value higher or equal to parameter
	float clamp(float value, float min, float max);	// Return parameter limited by the given range
	float floor(float value);						// Return highest integer value lower or equal to parameter
	float squareRoot(float value);					// Return square root of parameter
	float wrap(float value, float min, float max);	// Return parameter as value inside the given range
}

#endif // !LIBMATH_ARITHMETIC_H_
