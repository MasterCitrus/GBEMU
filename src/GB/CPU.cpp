#include "CPU.h"
#include "Memory.h"
#include "Stats.h"
#include <cstdio>
#include <cstdlib>
#include <format>

CPU::CPU(Memory* memory)
	: memory(memory)
{
	stats.PC = &PC;
	stats.OP = &OP;
	stats.registers = &registers;
	stats.cycles = &cycles;
	//outputLog.open("logging\\log.txt");
	//outputRegisters.open("logging\\registers.txt");
	//outputFlow.open("logging\\flow.txt");
}

CPU::~CPU()
{
	//outputLog.close();
	//outputRegisters.close();
	//outputFlow.close();
}

u8 CPU::FetchByte()
{
	u8 val = memory->Read(PC++);
	//outputflow << "Byte: " << std::format("{:#04X}", val) << "\n";
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
	

	int cycleAmount = 0;

	switch (OP)
	{
		case 0x00:
			//outputflow << " | NOP\n";
			cycleAmount = 4;
			break;
		case 0x04:
			//outputflow << " | INC B\n";
			if (((registers.B & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.B + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.B++;
			cycleAmount = 4;
			break;
		case 0x05:
			//outputflow << " | DEC B\n";
			if (((registers.B & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.B - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.B--;
			cycleAmount = 4;
			break;
		case 0x06:
			//outputflow << " | LD B, d8\n";
			registers.B = FetchByte();
			cycleAmount = 8;
			break;
		case 0x0C:
			//outputflow << " | INC C\n";
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
		case 0x0D:
			//outputflow << " | DEC C\n";
			if (((registers.C & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.C - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.C--;
			cycleAmount = 4;
			break;
		case 0x0E:
			//outputflow << " | LD C, d8\n";
			registers.C = FetchByte();
			cycleAmount = 8;
			break;
		case 0x11:
			//outputflow << " | LD DE, d16\n";
			registers.DE = FetchWord();
			cycleAmount = 12;
			break;
		case 0x13:
			//outputflow << " | INC DE\n";
			registers.DE++;
			cycleAmount = 8;
			break;
		case 0x14:
			//outputflow << " | INC D\n";
			if (((registers.D & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.D + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.D++;
			cycleAmount = 4;
			break;
		case 0x15:
			//outputflow << " | DEC D\n";
			if (((registers.D & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.D - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.D--;
			cycleAmount = 4;
			break;
		case 0x16:
			//outputflow << " | LD D, d8\n";
			registers.D = FetchByte();
			cycleAmount = 8;
			break;
		case 0x17:
		{
			//outputflow << " | RLA\n";
			bool carry = registers.GetCarryFlag();
			bool newCarry = (registers.A >> 7) & (u8)1;
			registers.SetCarryFlag(newCarry);
			registers.A = (registers.A << 1) | carry;
			registers.SetHalfCarryFlag(false);
			registers.SetSubtractFlag(false);
			registers.SetZeroFlag(false);
			cycleAmount = 4;
			break;
		}
		case 0x18:
		{
			//outputflow << " | JP s8\n";
			s8 offset = FetchByte();
			PC += offset;
			cycleAmount = 12;
			break;
		}
		case 0x1A:
			//outputflow << " | LD A, (DE)\n";
			registers.A = memory->Read(registers.DE);
			cycleAmount = 8;
			break;
		case 0x1C:
			//outputflow << " | INC E\n";
			if (((registers.E & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.E + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.E++;
			cycleAmount = 4;
			break;
		case 0x1D:
			//outputflow << " | DEC E\n";
			if (((registers.E & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.E - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.E--;
			cycleAmount = 4;
			break;
		case 0x1E:
			//outputflow << " | LD E, d8\n";
			registers.E = FetchByte();
			cycleAmount = 8;
			break;
		case 0x20:
		{
			//outputflow << " | JR NZ, s8\n";
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
			//outputflow << " | LD HL, d16\n";
			registers.HL = FetchWord();
			cycleAmount = 12;
			break;
		case 0x22:
			//outputflow << " | LD (HL+), A\n";
			memory->Write(registers.HL, registers.A);
			registers.HL++;
			cycleAmount = 8;
			break;
		case 0x23:
			//outputflow << " | INC HL\n";
			registers.HL++;
			cycleAmount = 8;
			break;
		case 0x24:
			//outputflow << " | INC H\n";
			if (((registers.H & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.H + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.H++;
			cycleAmount = 4;
			break;
		case 0x25:
			//outputflow << " | DEC H\n";
			if (((registers.H & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.H - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.H--;
			cycleAmount = 4;
			break;
		case 0x26:
			//outputflow << " | LD H, d8\n";
			registers.H = FetchByte();
			cycleAmount = 8;
			break;
		case 0x28:
		{
			//outputflow << " | JP Z, s8\n";
			s8 offset = static_cast<s8>(FetchByte());
			if (registers.GetZeroFlag())
			{
				PC += offset;
				cycleAmount = 12;
				break;
			}
			cycleAmount = 8;
			break;
		}
		case 0x2B:
			//outputflow << " | DEC HL\n";
			registers.HL--;
			cycleAmount = 8;
			break;
		case 0x2C:
			//outputflow << " | INC L\n";
			if (((registers.L & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.L + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.L++;
			cycleAmount = 4;
			break;
		case 0x2D:
			//outputflow << " | DEC L\n";
			if (((registers.L & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.L - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.L--;
			cycleAmount = 4;
			break;
		case 0x2E:
			//outputflow << " | LD L, d8\n";
			registers.L = FetchByte();
			cycleAmount = 8;
			break;
		case 0x31:
			//outputflow << " | LD SP, d16\n";
			SP = FetchWord();
			cycleAmount = 12;
			break;
		case 0x32:
			//outputflow << " | LD (HL-), A\n";
			memory->Write(registers.HL, registers.A);
			registers.HL--;
			cycleAmount = 8;
			break;
		case 0x34:
		{
			//outputflow << " | INC (HL)\n";
			u8 value = memory->Read(registers.HL);
			if (((value & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((value + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			value++;
			memory->Write(registers.HL, value);
			cycleAmount = 4;
			break;
		}
		case 0x35:
		{
			//outputflow << " | DEC (HL)\n";
			u8 value = memory->Read(registers.HL);
			if (((value & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((value - 1) == 0);
			registers.SetSubtractFlag(true);
			value--;
			memory->Write(registers.HL, value);
			cycleAmount = 4;
			break;
		}
		case 0x3C:
			//outputflow << " | INC A\n";
			if (((registers.A & 0x0F) + 1) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + 1) == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			registers.SetSubtractFlag(false);
			registers.A++;
			cycleAmount = 4;
			break;
		case 0x3D:
			//outputflow << " | DEC A\n";
			if (((registers.A & 0x0F) < (1 & 0x0F)))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			registers.SetZeroFlag((registers.A - 1) == 0);
			registers.SetSubtractFlag(true);
			registers.A--;
			cycleAmount = 4;
			break;
		case 0x3E:
			//outputflow << " | LD A, d8\n";
			registers.A = FetchByte();
			cycleAmount = 8;
			break;
		case 0x40:
			//outputflow << " | LD B, B\n";
			cycleAmount = 4;
			break;
		case 0x41:
			//outputflow << " | LD B, C\n";
			registers.B = registers.C;
			cycleAmount = 4;
			break;
		case 0x42:
			//outputflow << " | LD B, D\n";
			registers.B = registers.D;
			cycleAmount = 4;
			break;
		case 0x43:
			//outputflow << " | LD B, E\n";
			registers.B = registers.E;
			cycleAmount = 4;
			break;
		case 0x44:
			//outputflow << " | LD B, H\n";
			registers.B = registers.H;
			cycleAmount = 4;
			break;
		case 0x45:
			//outputflow << " | LD B, L\n";
			registers.B = registers.L;
			cycleAmount = 4;
			break;
		case 0x46:
			//outputflow << " | LD B, (HL)\n";
			registers.B = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x47:
			//outputflow << " | LD C, B\n";
			registers.C = registers.B;
			cycleAmount = 4;
			break;
		case 0x48:
			//outputflow << " | LD C, C\n";
			cycleAmount = 4;
			break;
		case 0x49:
			//outputflow << " | LD C, D\n";
			registers.C = registers.D;
			cycleAmount = 4;
			break;
		case 0x4A:
			//outputflow << " | LD C, E\n";
			registers.C = registers.E;
			cycleAmount = 4;
			break;
		case 0x4B:
			//outputflow << " | LD C, H\n";
			registers.C = registers.H;
			cycleAmount = 4;
			break;
		case 0x4C:
			//outputflow << " | LD C, L\n";
			registers.C = registers.H;
			cycleAmount = 4;
			break;
		case 0x4D:
			//outputflow << " | LD C, L\n";
			registers.C = registers.L;
			cycleAmount = 4;
			break;
		case 0x4E:
			//outputflow << " | LD C, (HL)\n";
			registers.C = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x4F:
			//outputflow << " | LD C, A\n";
			registers.C = registers.A;
			cycleAmount = 4;
			break;
		case 0x50:
			//outputflow << " | LD D, B\n";
			registers.D = registers.B;
			cycleAmount = 4;
			break;
		case 0x51:
			//outputflow << " | LD D, C\n";
			registers.D = registers.C;
			cycleAmount = 4;
			break;
		case 0x52:
			//outputflow << " | LD D, D\n";
			cycleAmount = 4;
			break;
		case 0x53:
			//outputflow << " | LD D, E\n";
			registers.D = registers.E;
			cycleAmount = 4;
			break;
		case 0x54:
			//outputflow << " | LD D, H\n";
			registers.D = registers.H;
			cycleAmount = 4;
			break;
		case 0x55:
			//outputflow << " | LD D, L\n";
			registers.D = registers.L;
			cycleAmount = 4;
			break;
		case 0x56:
			//outputflow << " | LD D, (HL)\n";
			registers.D = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x57:
			//outputflow << " | LD D, A\n";
			registers.D = registers.A;
			cycleAmount = 4;
			break;
		case 0x58:
			//outputflow << " | LD E, B\n";
			registers.E = registers.B;
			cycleAmount = 4;
			break;
		case 0x59:
			//outputflow << " | LD E, C\n";
			registers.E = registers.C;
			cycleAmount = 4;
			break;
		case 0x5A:
			//outputflow << " | LD E, D\n";
			registers.E = registers.D;
			cycleAmount = 4;
			break;
		case 0x5B:
			//outputflow << " | LD E, E\n";
			cycleAmount = 4;
			break;
		case 0x5C:
			//outputflow << " | LD E, H\n";
			registers.E = registers.H;
			cycleAmount = 4;
			break;
		case 0x5D:
			//outputflow << " | LD E, L\n";
			registers.E = registers.L;
			cycleAmount = 4;
			break;
		case 0x5E:
			//outputflow << " | LD E, (HL)\n";
			registers.E = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x5F:
			//outputflow << " | LD E, A\n";
			registers.E = registers.A;
			cycleAmount = 4;
			break;
		case 0x60:
			//outputflow << " | LD H, B\n";
			registers.H = registers.B;
			cycleAmount = 4;
			break;
		case 0x61:
			//outputflow << " | LD H, C\n";
			registers.H = registers.C;
			cycleAmount = 4;
			break;
		case 0x62:
			//outputflow << " | LD H, D\n";
			registers.H = registers.D;
			cycleAmount = 4;
			break;
		case 0x63:
			//outputflow << " | LD H, E\n";
			registers.H = registers.E;
			cycleAmount = 4;
			break;
		case 0x64:
			//outputflow << " | LD H, H\n";
			cycleAmount = 4;
			break;
		case 0x65:
			//outputflow << " | LD H, L\n";
			registers.H = registers.L;
			cycleAmount = 4;
			break;
		case 0x66:
			//outputflow << " | LD H, (HL)\n";
			registers.H = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x67:
			//outputflow << " | LD H, A\n";
			registers.H = registers.A;
			cycleAmount = 4;
			break;
		case 0x68:
			//outputflow << " | LD L, B\n";
			registers.L = registers.B;
			cycleAmount = 4;
			break;
		case 0x69:
			//outputflow << " | LD L, C\n";
			registers.L = registers.C;
			cycleAmount = 4;
			break;
		case 0x6A:
			//outputflow << " | LD L, D\n";
			registers.L = registers.D;
			cycleAmount = 4;
			break;
		case 0x6B:
			//outputflow << " | LD L, E\n";
			registers.L = registers.E;
			cycleAmount = 4;
			break;
		case 0x6C:
			//outputflow << " | LD L, H\n";
			registers.L = registers.H;
			cycleAmount = 4;
			break;
		case 0x6D:
			//outputflow << " | LD L, L\n";
			cycleAmount = 4;
			break;
		case 0x6E:
			//outputflow << " | LD L, (HL)\n";
			registers.L = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x6F:
			//outputflow << " | LD L, A\n";
			registers.L = registers.A;
			cycleAmount = 4;
			break;
		case 0x70:
			//outputflow << " | LD (HL), B\n";
			memory->Write(registers.HL, registers.B);
			cycleAmount = 8;
			break;
		case 0x71:
			//outputflow << " | LD (HL), C\n";
			memory->Write(registers.HL, registers.C);
			cycleAmount = 8;
			break;
		case 0x72:
			//outputflow << " | LD (HL), D\n";
			memory->Write(registers.HL, registers.D);
			cycleAmount = 8;
			break;
		case 0x73:
			//outputflow << " | LD (HL), E\n";
			memory->Write(registers.HL, registers.E);
			cycleAmount = 8;
			break;
		case 0x74:
			//outputflow << " | LD (HL), H\n";
			memory->Write(registers.HL, registers.H);
			cycleAmount = 8;
			break;
		case 0x75:
			//outputflow << " | LD (HL), L\n";
			memory->Write(registers.HL, registers.L);
			cycleAmount = 8;
			break;
		case 0x77:
			//outputflow << " | LD (HL), A\n";
			memory->Write(registers.HL, registers.A);
			cycleAmount = 8;
			break;
		case 0x78:
			//outputflow << " | LD A, B\n";
			registers.A = registers.B;
			cycleAmount = 4;
			break;
		case 0x79:
			//outputflow << " | LD A, C\n";
			registers.A = registers.C;
			cycleAmount = 4;
			break;
		case 0x7A:
			//outputflow << " | LD A, D\n";
			registers.A = registers.D;
			cycleAmount = 4;
			break;
		case 0x7B:
			//outputflow << " | LD A, E\n";
			registers.A = registers.E;
			cycleAmount = 4;
			break;
		case 0x7C:
			//outputflow << " | LD A, H\n";
			registers.A = registers.H;
			cycleAmount = 4;
			break;
		case 0x7D:
			//outputflow << " | LD A, L\n";
			registers.A = registers.L;
			cycleAmount = 4;
			break;
		case 0x7E:
			//outputflow << " | LD A, (HL)\n";
			registers.A = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x7F:
			//outputflow << " | LD A, A\n";
			cycleAmount = 4;
			break;
		case 0x80:
		{
			//outputflow << " | ADD A, B\n";
			u8 result = registers.A + registers.B;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.B & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.B) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x81:
		{
			//outputflow << " | ADD A, C\n";
			u8 result = registers.A + registers.C;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.C & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.C) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x82:
		{
			//outputflow << " | ADD A, D\n";
			u8 result = registers.A + registers.D;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.D & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.D) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x83:
		{
			//outputflow << " | ADD A, E\n";
			u8 result = registers.A + registers.E;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.E & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.E) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x84:
		{
			//outputflow << " | ADD A, H\n";
			u8 result = registers.A + registers.H;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.H & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.H) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x85:
		{
			//outputflow << " | ADD A, L\n";
			u8 result = registers.A + registers.L;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.L & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.L) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x86:
		{
			//outputflow << " | ADD A, (HL)\n";
			u8 value = memory->Read(registers.HL);
			u8 result = registers.A + value;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (value & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + value) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			registers.A = result;
			break;
		}
		case 0x87:
		{
			//outputflow << " | ADD A, A\n";
			u8 result = registers.A + registers.A;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.A & 0x0F)) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.A) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x88:
		{
			//outputflow << " | ADC A, B\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.B + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.B & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.B + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x89:
		{
			//outputflow << " | ADC A, C\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.C + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.C & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.C + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x8A:
		{
			//outputflow << " | ADC A, D\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.D + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.D & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.D + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x8B:
		{
			//outputflow << " | ADC A, E\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.E + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.E & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.E + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x8C:
		{
			//outputflow << " | ADC A, H\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.H + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.H & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.H + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x8D:
		{
			//outputflow << " | ADC A, L\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.L + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.L & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.L + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x8E:
		{
			//outputflow << " | ADC A, (HL)\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 value = memory->Read(registers.HL);
			u8 result = registers.A + value + carry;
			registers.SetZeroFlag((result) == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (value & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + value + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			registers.A = result;
			break;
		}
		case 0x8F:
		{
			//outputflow << " | ADC A, A\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A + registers.A + carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(false);
			if (((registers.A & 0x0F) + (registers.A & 0x0F) + carry) > 0x0F)
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if ((registers.A + registers.A + carry) > 0xFF)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x90:
		{
			//outputflow << " | SUB B\n";
			u8 result = registers.A - registers.B;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.B & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.B)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x91:
		{
			//outputflow << " | SUB C\n";
			u8 result = registers.A - registers.C;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.C & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.C)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x92:
		{
			//outputflow << " | SUB D\n";
			u8 result = registers.A - registers.D;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.D & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.D)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x93:
		{
			//outputflow << " | SUB E\n";
			u8 result = registers.A - registers.E;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.E & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.E)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x94:
		{
			//outputflow << " | SUB H\n";
			u8 result = registers.A - registers.H;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.H & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.H)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x95:
		{
			//outputflow << " | SUB L\n";
			u8 result = registers.A - registers.L;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.L & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.L)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x96:
		{
			//outputflow << " | SUB (HL)\n";
			u8 value = memory->Read(registers.HL);
			u8 result = registers.A - value;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (value & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < value)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			registers.A = result;
			break;
		}
		case 0x97:
		{
			//outputflow << " | SUB A\n";
			u8 result = registers.A - registers.A;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < (registers.A & 0x0F))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.A)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x98:
		{
			//outputflow << " | SBC B\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.B - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.B & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.B + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x99:
		{
			//outputflow << " | SBC C\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.C - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.C & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.C + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x9A:
		{
			//outputflow << " | SBC D\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.D - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.D & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.D + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x9B:
		{
			//outputflow << " | SBC E\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.E - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.E & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.E + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x9C:
		{
			//outputflow << " | SBC H\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.H - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.H & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.H + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x9D:
		{
			//outputflow << " | SBC L\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.L - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.L & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.L + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0x9E:
		{
			//outputflow << " | SBC (HL)\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 value = memory->Read(registers.HL);
			u8 result = registers.A - value - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((value & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (value + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			registers.A = result;
			break;
		}
		case 0x9F:
		{
			//outputflow << " | SBC A\n";
			u8 carry = (u8)registers.GetCarryFlag();
			u8 result = registers.A - registers.A - carry;
			registers.SetZeroFlag(result == 0);
			registers.SetSubtractFlag(true);
			if ((registers.A & 0x0F) < ((registers.A & 0x0F) + carry))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < (registers.A + carry))
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			registers.A = result;
			break;
		}
		case 0xA0:
			//outputflow << " | AND B\n";
			registers.A &= registers.B;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA1:
			//outputflow << " | AND C\n";
			registers.A &= registers.C;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA2:
			//outputflow << " | AND D\n";
			registers.A &= registers.D;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA3:
			//outputflow << " | AND E\n";
			registers.A &= registers.E;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA4:
			//outputflow << " | AND H\n";
			registers.A &= registers.H;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA5:
			//outputflow << " | AND L\n";
			registers.A &= registers.L;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA6:
			//outputflow << " | AND (HL)\n";
			registers.A &= memory->Read(registers.HL);
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 8;
			break;
		case 0xA7:
			//outputflow << " | AND A\n";
			registers.A &= registers.A;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA8:
			//outputflow << " | XOR B\n";
			registers.B = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA9:
			//outputflow << " | XOR C\n";
			registers.C = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAA:
			//outputflow << " | XOR D\n";
			registers.D = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAB:
			//outputflow << " | XOR E\n";
			registers.E = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAC:
			//outputflow << " | XOR H\n";
			registers.H = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAD:
			//outputflow << " | XOR L\n";
			registers.L = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAE:
			//outputflow << " | XOR (HL)\n";
			memory->Write(registers.HL, 0);
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 8;
			break;
		case 0xAF:
			//outputflow << " | XOR A\n";
			registers.A = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB0:
			//outputflow << " | OR B\n";
			registers.A |= registers.B;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB1:
			//outputflow << " | OR C\n";
			registers.A |= registers.C;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB2:
			//outputflow << " | OR D\n";
			registers.A |= registers.D;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB3:
			//outputflow << " | OR E\n";
			registers.A |= registers.E;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB4:
			//outputflow << " | OR H\n";
			registers.A |= registers.H;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB5:
			//outputflow << " | OR L\n";
			registers.A |= registers.L;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB6:
			//outputflow << " | OR (HL)\n";
			registers.A |= memory->Read(registers.HL);
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 8;
			break;
		case 0xB7:
			//outputflow << " | OR A\n";
			registers.A |= registers.A;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB8:
			//outputflow << " | CP B\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.B == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.B & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.B)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xB9:
			//outputflow << " | CP C\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.C == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.C & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.C)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xBA:
			//outputflow << " | CP D\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.D == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.D & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.D)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xBB:
			//outputflow << " | CP E\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.E == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.E & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.E)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xBC:
			//outputflow << " | CP H\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.H == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.H & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.H)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xBD:
			//outputflow << " | CP H\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.L == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.L & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.L)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xBE:
		{
			//outputflow << " | CP (HL)\n";
			u8 value = memory->Read(registers.HL);
			registers.SetSubtractFlag(true);
			if (registers.A - value == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (value & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < value)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			break;
		}
		case 0xBF:
			//outputflow << " | CP A\n";
			registers.SetSubtractFlag(true);
			if (registers.A - registers.A == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (registers.A & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < registers.A)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 4;
			break;
		case 0xC1:
			//outputflow << " | POP BC\n";
			registers.B = memory->Read(SP++);
			registers.C = memory->Read(SP++);
			cycleAmount = 12;
			break;
		case 0xC5:
			//outputflow << " | PUSH BC\n";
			memory->Write(--SP, registers.C);
			memory->Write(--SP, registers.B);
			cycleAmount = 24;
			break;
		case 0xC9:
		{
			//outputflow << " | RET\n";
			u8 low = memory->Read(SP++);
			u8 high = memory->Read(SP++);
			PC = (high << 8) | low;
			cycleAmount = 16;
			break;
		}
		case 0xCB:
		{
			//outputflow << " | PREFIX\n";
			cycleAmount = 4 + DecodeExtended();
			break;
		}
		case 0xCD:
		{
			//outputflow << " | CALL a16\n";
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
			//outputflow << " | LD (a8), A\n";
			u8 offset = FetchByte();
			memory->Write(0xFF00 + offset, registers.A);
			cycleAmount = 12;
			break;
		}
		case 0xE2:
			//outputflow << " | LD (C), A\n";
			memory->Write(0xFF00 + registers.C, registers.A);
			cycleAmount = 8;
			break;
		case 0xEA:
		{
			//outputflow << " | LD a16, A\n";
			u16 address = FetchWord();
			memory->Write(address, registers.A);
			cycleAmount = 16;
			break;
		}
		case 0xF0:
		{
			//outputflow << " | LD A, (a8)\n";
			u8 offset = FetchByte();
			registers.A = memory->Read(0xFF00 + offset);
			cycleAmount = 12;
			break;
		}
		case 0xFE:
		{
			//outputflow << " | CP d8\n";
			u8 val = FetchByte();
			registers.SetSubtractFlag(true);
			if (registers.A - val == 0)
			{
				registers.SetZeroFlag(true);
			}
			else
			{
				registers.SetZeroFlag(false);
			}
			if ((registers.A & 0xF) < (val & 0xF))
			{
				registers.SetHalfCarryFlag(true);
			}
			else
			{
				registers.SetHalfCarryFlag(false);
			}
			if (registers.A < val)
			{
				registers.SetCarryFlag(true);
			}
			else
			{
				registers.SetCarryFlag(false);
			}
			cycleAmount = 8;
			break;
		}
		default:
			std::printf("Unsupported Instruction : 0x%02x at 0x%04x\n\n\n", OP, PC - 1);
			std::exit(EXIT_FAILURE);
			cycleAmount = 4;
			break;
	}

	//outputLog << "[Instruction] OP: " << opcode << " | PC: " << program << "\n";
	//outputflow << "[Registers] - OP "
		//<< "\nAF: " << std::format("{:#04X}", registers.AF) << "\nA: " << std::format("{:#04X}", registers.A) << "\nF: " << std::format("{:#04X}", registers.F)
		//<< "\nBC: " << std::format("{:#04X}", registers.BC) << "\nB: " << std::format("{:#04X}", registers.B) << "\nC: " << std::format("{:#04X}", registers.C)
		//<< "\nDE: " << std::format("{:#04X}", registers.DE) << "\nD: " << std::format("{:#04X}", registers.D) << "\nE: " << std::format("{:#04X}", registers.E)
		//<< "\nHL: " << std::format("{:#04X}", registers.HL) << "\nH: " << std::format("{:#04X}", registers.H) << "\nL: " << std::format("{:#04X}", registers.L)
		//<< "\nPC: " << std::format("{:#04X}", PC) << "\nSP: " << std::format("{:#04X}", SP) << "\n";
	//outputflow << "Next Address: " << std::format("{:#04X}", PC) << "\n\n\n";

	return cycleAmount;
}

int CPU::DecodeExtended()
{
	u8 opcode = FetchByte();
	//outputflow << "Prefix: " << std::format("{:#04X}", opcode);

	std::string prefix = std::format("{:#04X}", opcode);
	std::string program = std::format("{:#04X}", PC);
	//outputLog << "[Prefix Instruction] OP: " << prefix << " | PC: " << program << "\n";

	int cycleAmount = 0;

	switch (opcode)
	{
		case 0x11:
		{
			//outputflow << " | RL C\n";
			bool carry = registers.GetCarryFlag();
			bool newCarry = ((registers.C >> 7) & (u8)1) ? 1 : 0;
			registers.SetCarryFlag(newCarry);
			registers.C = (registers.C << 1) | carry;
			registers.SetZeroFlag(registers.C == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			cycleAmount = 8;
			break;
		}
		case 0x7C:
			//outputflow << " | BIT 7 H\n";
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
			//outputflow << " | SET 1, E\n";
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

	//outputflow << "Address: " << std::format("{:#04X}", PC) << "\n";
	if (PC == 0x100)
	{
		std::printf("Boot Rom finished\n");
	}

	OP = FetchByte();
	//outputflow << "Instruction: " << std::format("{:#04X}", OP);

	cycles = Decode();

	return cycles;
}
