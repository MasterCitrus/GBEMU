#include "CPU.h"
#include "Memory.h"
#include <cstdio>
#include <cstdlib>

CPU::CPU(Memory* memory)
	: memory(memory)
{
}

u8 CPU::FetchByte()
{
	return memory->Read(PC++);
}

u16 CPU::FetchWord()
{
	u8 low = FetchByte();
	u8 high = FetchByte();
	return (high << 8) | low;
}

int CPU::Decode()
{
	switch (OP)
	{
		case 0x00:
			std::printf("NOP | 0x%02x at 0x%04x\n\n\n", OP, PC);
			return 4;
		case 0x21:
			std::printf("LD HL, d16 | 0x%02x at 0x%04x\n\n\n", OP, PC);
			registers.HL = FetchWord();
			return 12;
		case 0x31:
			std::printf("LD SP, d16 | 0x%02x at 0x%04x\n\n\n", OP, PC);
			SP = FetchWord();
			return 12;
		case 0x32:
			std::printf("LD (HL-), A | 0x%02x at 0x%04x\n\n\n", OP, PC);
			memory->Write(registers.HL, registers.A);
			registers.HL--;
			return 8;
		case 0xAF:
			std::printf("XOR A | 0x%02x at 0x%04x\n\n\n", OP, PC);
			registers.A = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			return 4;
		case 0xCB:
			std::printf("PREFIX | 0x%02x at 0x%04x\n\n\n", OP, PC);
			u8 opcode = FetchByte();
			return 4 + DecodeExtended(opcode);
		default:
			std::printf("Unsupported Instruction : 0x%02x at 0x%04x\n\n\n", OP, PC);
			std::exit(EXIT_FAILURE);
			return 4;
	}
}

int CPU::DecodeExtended(u8 opcode)
{
	return 0;
}

void CPU::Reset()
{
	registers = Registers();
	cycles = 0;
	halted = false;
	ime = false;
	SP = 0x0;
	PC = 0x0;
}

int CPU::Step()
{
	if (halted) return 4;

	OP = FetchByte();

	return Decode();
}
