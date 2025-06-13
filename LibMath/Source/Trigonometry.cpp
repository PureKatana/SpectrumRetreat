#include "LibMath/Trigonometry.h"
#include "LibMath/Constants.h"

float LibMath::sin(const Radian& rad)
{
    return std::sin(rad.radian());
}

float LibMath::cos(const Radian& rad)
{
    return std::cos(rad.radian());
}

float LibMath::tan(const Radian& rad)
{
    return std::tan(rad.radian());
}

LibMath::Radian LibMath::asin(float value)
{
    return Radian(std::asin(value));
}

LibMath::Radian LibMath::acos(float value)
{
    return Radian(std::acos(value));
}

LibMath::Radian LibMath::atan(float value)
{
    return Radian(std::atan(value));
}

LibMath::Radian LibMath::atan(float y, float x)
{
    return Radian(std::atan2(y, x));
}

