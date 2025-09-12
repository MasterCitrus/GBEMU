#pragma once

#include "Types.h"

struct Registers;

struct CPUStats
{
	u16* PC;
	u8* OP;
	Registers* registers;
	int* cycles;
};