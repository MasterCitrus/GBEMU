#pragma once

#include "Register.h"

class Memory;

class CPU
{
public:
	CPU(Memory* memory);

	u8 FetchByte();
	u16 FetchWord();

	int Decode();
	int DecodeExtended();

	void Reset();

	int Step();

private:
	Registers registers;
	Memory* memory;
	int cycles = 0;
	u16 SP = 0x0;
	u16 PC = 0x0;
	u8 OP;
	bool ime = false;;
	bool halted = false;
};