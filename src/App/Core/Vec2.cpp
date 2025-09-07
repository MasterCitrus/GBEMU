#include "Vec2.h"
#include <cmath>

Vec2::operator iVec2()
{
	return iVec2(std::floor(x), std::floor(y));
}

iVec2::operator Vec2()
{
	return Vec2(x, y);
}
