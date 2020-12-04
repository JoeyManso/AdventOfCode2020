#ifndef ADVENT_MATH_H
#define ADVENT_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

template <typename T>
inline T sqr(T x)
{
	return(x * x);
}

inline double ToRadians(double degrees)
{
	return(degrees * (M_PI / 180));
}

inline double ToDegrees(double radians)
{
	return(radians * (180.0 / M_PI));
}

inline int ToInt(char c)
{
    return(c - '0');
}

template <typename T>
inline T Min(const T& left, const T& right)
{
	return std::min(left, right);
}

template <typename T>
inline T Max(const T& left, const T& right)
{
	return std::max(left, right);
}

// Inclusive clamp
template <typename T>
inline T Clamp(const T& n, const T& lower, const T& upper)
{
    return Max(lower, Min(n, upper));
}

template <typename T>
inline T InRangeInclusive(const T& n, const T& lower, const T& upper)
{
    return(n >= lower && n <= upper);
}

template <typename T>
inline T InRangeExclusive(const T& n, const T& lower, const T& upper)
{
    return(n > lower && n < upper);
}

#endif // ADVENT_MATH_H