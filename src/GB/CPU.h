#pragma once

#include "Register.h"
#include "Stats.h"
#include <fstream>

class Memory;

class CPU
{
public:
	CPU(Memory* memory);
	~CPU();

	u8 FetchByte();
	u16 FetchWord();

	int Decode();
	int DecodeExtended();

	void PushStack();
	void PopStack();

	void Reset();

	int Step();

	CPUStats GetStats() const { return stats; }

private:
	std::ofstream outputLog;
	std::ofstream outputRegisters;
	std::ofstream outputFlow;
	CPUStats stats;
	Registers registers;
	Memory* memory;
	int cycles = 0;
	u16 SP = 0x0;
	u16 PC = 0x0;
	u8 OP;
	bool ime = false;;
	bool halted = false;
};