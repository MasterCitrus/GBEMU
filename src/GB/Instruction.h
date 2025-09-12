#pragma once
#include "Types.h"

enum class RegisterTarget
{
	A,
	F,
	B,
	C,
	D,
	E,
	H,
	L,
	AF,
	BC,
	DE,
	HL
};

struct Instruction
{
	int numBytes;
	int cycles;

};