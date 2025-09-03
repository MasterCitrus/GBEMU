#include "CPU.h"
#include "Memory.h"
#include <cstdio>
#include <cstdlib>
#include <format>

CPU::CPU(Memory* memory)
	: memory(memory)
{
	outputLog.open("log.txt");
	outputRegisters.open("registers.txt");
}

CPU::~CPU()
{
	outputLog.close();
	outputRegisters.close();
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
	std::string opcode = std::format("{:#04X}", OP);
	std::string program = std::format("{:#04X}", PC);
	

	int cycleAmount =0;

	switch (OP)
	{
		case 0x00:
			//std::printf("NOP | 0x%02x at 0x%04x\n\n\n", OP, PC);
			cycleAmount = 4;
			break;
		case 0x05:
			if (((registers.B & 0x0F) - 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.B - 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.B--;
			cycleAmount = 4;
			break;
		case 0x06:
			registers.B = FetchByte();
			cycleAmount = 8;
			break;
		case 0x0E:
			registers.C = FetchByte();
			cycleAmount = 8;
			break;
		case 0x0C:
			if (((registers.C & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.C + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.C++;
			cycleAmount = 4;
			break;
		case 0x11:
			registers.DE = FetchWord();
			cycleAmount = 12;
			break;
		case 0x17:
		{
			bool carry = registers.GetCarryFlag();
			bool newCarry = (registers.A << 1) & (u8)1;
			registers.SetCarryFlag(newCarry);
			registers.A = (registers.A << 1) | carry;
			registers.SetHalfCarryFlag(false);
			registers.SetSubtractFlag(false);
			registers.SetZeroFlag(false);
			cycleAmount = 4;
			break;
		}
		case 0x1A:
			registers.A = memory->Read(registers.DE);
			cycleAmount = 8;
			break;
		case 0x20:
		{
			s8 offset = static_cast<s8>(FetchByte());
			//std::printf("%i\n", offset);
			if (!registers.GetZeroFlag())
			{
				PC += offset;
				cycleAmount = 12;
				break;
			}
			//PC++;
			cycleAmount = 8;
			break;
		}
		case 0x21:
			//std::printf("LD HL, d16 | 0x%02x at 0x%04x\n\n\n", OP, PC);
			registers.HL = FetchWord();
			cycleAmount = 12;
			break;
		case 0x22:
			memory->Write(registers.HL, registers.A);
			registers.HL++;
			cycleAmount = 8;
			break;
		case 0x26:
			registers.H = FetchByte();
			break;
		case 0x31:
			//std::printf("LD SP, d16 | 0x%02x at 0x%04x\n\n\n", OP, PC);
			SP = FetchWord();
			cycleAmount = 12;
			break;
		case 0x32:
			//std::printf("LD (HL-), A | 0x%02x at 0x%04x\n\n\n", OP, PC);
			memory->Write(registers.HL, registers.A);
			registers.HL--;
			cycleAmount = 8;
			break;
		case 0x3E:
			registers.A = FetchByte();
			cycleAmount = 8;
			break;
		case 0x4F:
			registers.C = registers.A;
			cycleAmount = 4;
			break;
		case 0x77:
			memory->Write(registers.HL, registers.A);
			cycleAmount = 8;
			break;
		case 0xAF:
			//std::printf("XOR A | 0x%02x at 0x%04x\n\n\n", OP, PC);
			registers.A = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xC1:
			registers.B = memory->Read(SP++);
			registers.C = memory->Read(SP++);
			cycleAmount = 12;
			break;
		case 0xC5:
			memory->Write(SP--, registers.C);
			memory->Write(SP--, registers.B);
			cycleAmount = 24;
			break;
		case 0xCB:
		{
			//std::printf("PREFIX | 0x%02x at 0x%04x\n\n\n", OP, PC);
			cycleAmount = 4 + DecodeExtended();
			break;
		}
		case 0xCD:
		{
			u16 address = FetchWord();
			u8 low = PC & 0xFF;
			u8 high = PC >> 8;
			memory->Write(SP--, high);
			memory->Write(SP--, low);
			PC = address;
			cycleAmount = 24;
			break;
		}
		case 0xE0:
		{
			u8 offset = FetchByte();
			memory->Write(0xFF00 + offset, registers.A);
			cycleAmount = 12;
			break;
		}
		case 0xE2:
			memory->Write(0xFF00 + registers.C, registers.A);
			cycleAmount = 8;
			break;
		default:
			std::printf("Unsupported Instruction : 0x%02x at 0x%04x\n\n\n", OP, PC);
			std::exit(EXIT_FAILURE);
			cycleAmount = 4;
			break;
	}

	outputLog << "[Instruction] OP: " << opcode << " | PC: " << program << "\n";
	outputRegisters << "[Registers] - OP " << std::format("{:#02X}", OP) << " | Address: " << std::format("{:#04X}", PC)
		<< "\nAF: " << std::format("{:#04X}", registers.AF) << "\nA: " << std::format("{:#04X}", registers.A) << "\nF: " << std::format("{:#04X}", registers.F)
		<< "\nBC: " << std::format("{:#04X}", registers.BC) << "\nB: " << std::format("{:#04X}", registers.B) << "\nC: " << std::format("{:#04X}", registers.C)
		<< "\nDE: " << std::format("{:#04X}", registers.DE) << "\nD: " << std::format("{:#04X}", registers.D) << "\nE: " << std::format("{:#04X}", registers.E)
		<< "\nHL: " << std::format("{:#04X}", registers.HL) << "\nH: " << std::format("{:#04X}", registers.H) << "\nL: " << std::format("{:#04X}", registers.L)
		<< "\nPC: " << std::format("{:#04X}", PC) << "\nSP: " << std::format("{:#04X}", SP) << "\n\n";

	return cycleAmount;
}

int CPU::DecodeExtended()
{
	u8 opcode = FetchByte();

	std::string prefix = std::format("{:#04X}", opcode);
	std::string program = std::format("{:#04X}", PC);
	outputLog << "[Prefix Instruction] OP: " << prefix << " | PC: " << program << "\n";

	int cycleAmount = 0;

	switch (opcode)
	{
		case 0x11:
		{
			bool carry = registers.GetCarryFlag();
			bool newCarry = (registers.C >> 0) & (u8)1;
			registers.SetCarryFlag(newCarry);
			registers.C = (registers.C << 1) | carry;
			if (registers.C == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			cycleAmount = 8;
			break;
		}
		case 0x7C:
			if (((registers.H >> 7) & (u8)1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			cycleAmount = 8;
			break;
		case 0xCB:
			registers.E = registers.E | 1 << 1;
			cycleAmount = 8;
			break;
		default:
			std::printf("Unsupported Prefix Instruction : 0x%02x at 0x%04x\n\n\n", opcode, PC);
			std::exit(EXIT_FAILURE);
			cycleAmount = 8;
			break;
	}

	return cycleAmount;
}

void CPU::PushStack()
{

}

void CPU::PopStack()
{

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
