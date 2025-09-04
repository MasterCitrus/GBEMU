#include "CPU.h"
#include "Memory.h"
#include <cstdio>
#include <cstdlib>
#include <format>

CPU::CPU(Memory* memory)
	: memory(memory)
{
	outputLog.open("logging\\log.txt");
	outputRegisters.open("logging\\registers.txt");
	outputFlow.open("logging\\flow.txt");
}

CPU::~CPU()
{
	outputLog.close();
	outputRegisters.close();
	outputFlow.close();
}

u8 CPU::FetchByte()
{
	u8 val = memory->Read(PC++);
	outputFlow << "Byte: " << std::format("{:#04X}", val) << "\n";
	return val;
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
			outputFlow << " | NOP\n";
			cycleAmount = 4;
			break;
		case 0x05:
			outputFlow << " | DEC B\n";
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
			registers.SetSubtractFlag(true);
			registers.B--;
			cycleAmount = 4;
			break;
		case 0x06:
			outputFlow << " | LD B, d8\n";
			registers.B = FetchByte();
			cycleAmount = 8;
			break;
		case 0x0C:
			outputFlow << " | INC C\n";
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
		case 0x0E:
			outputFlow << " | LD C, d8\n";
			registers.C = FetchByte();
			cycleAmount = 8;
			break;
		case 0x11:
			outputFlow << " | LD DE, d16\n";
			registers.DE = FetchWord();
			cycleAmount = 12;
			break;
		case 0x13:
			outputFlow << " | INC DE\n";
			registers.DE++;
			cycleAmount = 8;
			break;
		case 0x17:
		{
			outputFlow << " | RLA\n";
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
			outputFlow << " | LD A, (DE)\n";
			registers.A = memory->Read(registers.DE);
			cycleAmount = 8;
			break;
		case 0x20:
		{
			outputFlow << " | JR NZ, s8\n";
			s8 offset = static_cast<s8>(FetchByte());
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
			outputFlow << " | LD HL, d16\n";
			registers.HL = FetchWord();
			cycleAmount = 12;
			break;
		case 0x22:
			outputFlow << " | LD (HL+), A\n";
			memory->Write(registers.HL, registers.A);
			registers.HL++;
			cycleAmount = 8;
			break;
		case 0x26:
			outputFlow << " | LD H, d8\n";
			registers.H = FetchByte();
			break;
		case 0x31:
			outputFlow << " | LD SP, d16\n";
			SP = FetchWord();
			cycleAmount = 12;
			break;
		case 0x32:
			outputFlow << " | LD (HL-), A\n";
			memory->Write(registers.HL, registers.A);
			registers.HL--;
			cycleAmount = 8;
			break;
		case 0x3E:
			outputFlow << " | LD A, d8\n";
			registers.A = FetchByte();
			cycleAmount = 8;
			break;
		case 0x4F:
			outputFlow << " | LD C, A\n";
			registers.C = registers.A;
			cycleAmount = 4;
			break;
		case 0x77:
			outputFlow << " | LD (HL), A\n";
			memory->Write(registers.HL, registers.A);
			cycleAmount = 8;
			break;
		case 0xAF:
			outputFlow << " | XOR A\n";
			registers.A = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xC1:
			outputFlow << " | POP BC\n";
			registers.B = memory->Read(SP++);
			registers.C = memory->Read(SP++);
			cycleAmount = 12;
			break;
		case 0xC5:
			outputFlow << " | PUSH BC\n";
			memory->Write(SP--, registers.C);
			memory->Write(SP--, registers.B);
			cycleAmount = 24;
			break;
		case 0xCB:
		{
			outputFlow << " | PREFIX\n";
			cycleAmount = 4 + DecodeExtended();
			break;
		}
		case 0xCD:
		{
			outputFlow << " | CALL a16\n";
			u16 address = FetchWord();
			u8 low = PC & 0xFF;
			u8 high = PC >> 8;
			memory->Write(--SP, high);
			memory->Write(--SP, low);
			PC = address;
			cycleAmount = 24;
			break;
		}
		case 0xE0:
		{
			outputFlow << " | LD (a8), A\n";
			u8 offset = FetchByte();
			memory->Write(0xFF00 + offset, registers.A);
			cycleAmount = 12;
			break;
		}
		case 0xE2:
			outputFlow << " | LD (C), A\n";
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
	outputFlow << "[Registers] - OP "
		<< "\nAF: " << std::format("{:#04X}", registers.AF) << "\nA: " << std::format("{:#04X}", registers.A) << "\nF: " << std::format("{:#04X}", registers.F)
		<< "\nBC: " << std::format("{:#04X}", registers.BC) << "\nB: " << std::format("{:#04X}", registers.B) << "\nC: " << std::format("{:#04X}", registers.C)
		<< "\nDE: " << std::format("{:#04X}", registers.DE) << "\nD: " << std::format("{:#04X}", registers.D) << "\nE: " << std::format("{:#04X}", registers.E)
		<< "\nHL: " << std::format("{:#04X}", registers.HL) << "\nH: " << std::format("{:#04X}", registers.H) << "\nL: " << std::format("{:#04X}", registers.L)
		<< "\nPC: " << std::format("{:#04X}", PC) << "\nSP: " << std::format("{:#04X}", SP) << "\n";
	outputFlow << "Next Address: " << std::format("{:#04X}", PC) << "\n\n\n";

	return cycleAmount;
}

int CPU::DecodeExtended()
{
	u8 opcode = FetchByte();
	outputFlow << "Prefix: " << std::format("{:#04X}", opcode);

	std::string prefix = std::format("{:#04X}", opcode);
	std::string program = std::format("{:#04X}", PC);
	outputLog << "[Prefix Instruction] OP: " << prefix << " | PC: " << program << "\n";

	int cycleAmount = 0;

	switch (opcode)
	{
		case 0x11:
		{
			outputFlow << " | RL C\n";
			bool carry = registers.GetCarryFlag();
			bool newCarry = ((registers.C >> 0) & (u8)1) ? 1 : 0;
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
			outputFlow << " | BIT 7 H\n";
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
			outputFlow << " | SET 1, E\n";
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

	outputFlow << "Address: " << std::format("{:#04X}", PC) << "\n";

	OP = FetchByte();
	outputFlow << "Instruction: " << std::format("{:#04X}", OP);

	return Decode();
}
