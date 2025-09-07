#pragma once

struct Vec2
{
	float x;
	float y;

	operator iVec2();
};

struct iVec2
{
	int x;
	int y;

	operator Vec2();
};