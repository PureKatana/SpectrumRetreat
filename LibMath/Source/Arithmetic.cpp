#include "LibMath/Arithmetic.h"
#include "LibMath/Constants.h"

bool LibMath::almostEqual(float a, float b)
{
    return std::fabs(a - b) < g_epsilon;
}

float LibMath::ceiling(float value)
{
    return std::ceil(value);
}

float LibMath::clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }   
    else if (value > max)
    {
        return max;
    }
    
    return value;
}

float LibMath::floor(float value)
{
    return std::floor(value);
}

float LibMath::squareRoot(float value)
{
    if (value < 0)
    {
        // Handle negative values gracefully
        return -1.0f; // Return an error value
    }
    return std::sqrt(value);
}

float LibMath::wrap(float value, float min, float max)
{
    float range = max - min;

    if (range <= 0)
    {
        return min; // Invalid range
    }
    // Use modulo operation to bring value within the range.
    value = std::fmod(value - min, range);
    if (value < 0.0f)
    {
        value += range;
    }

    return value + min;
}
