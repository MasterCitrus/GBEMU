#pragma once

#include "Register.h"

class CPU
{
public:
	CPU();

	int Step();
	int PrefixStep(u16 programCode);

private:
	u8 memory[0xFFFF] = {0};
	Registers registers;
};