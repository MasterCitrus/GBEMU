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
	

	int cycleAmount = 0;

	switch (OP)
	{
		case 0x00:
			outputFlow << " | NOP\n";
			cycleAmount = 4;
			break;
		case 0x04:
			outputFlow << " | INC B\n";
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
			outputFlow << " | DEC B\n";
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
		case 0x0D:
			outputFlow << " | DEC C\n";
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
		case 0x14:
			outputFlow << " | INC D\n";
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
			outputFlow << " | DEC D\n";
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
			outputFlow << " | LD D, d8\n";
			registers.D = FetchByte();
			cycleAmount = 8;
			break;
		case 0x17:
		{
			outputFlow << " | RLA\n";
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
			outputFlow << " | JP s8\n";
			s8 offset = FetchByte();
			PC += offset;
			cycleAmount = 12;
			break;
		}
		case 0x1A:
			outputFlow << " | LD A, (DE)\n";
			registers.A = memory->Read(registers.DE);
			cycleAmount = 8;
			break;
		case 0x1C:
			outputFlow << " | INC E\n";
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
			outputFlow << " | DEC E\n";
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
			outputFlow << " | LD E, d8\n";
			registers.E = FetchByte();
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
		case 0x23:
			outputFlow << " | INC HL\n";
			registers.HL++;
			cycleAmount = 8;
			break;
		case 0x24:
			outputFlow << " | INC H\n";
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
			outputFlow << " | DEC H\n";
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
			outputFlow << " | LD H, d8\n";
			registers.H = FetchByte();
			cycleAmount = 8;
			break;
		case 0x28:
		{
			outputFlow << " | JP Z, s8\n";
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
			outputFlow << " | DEC HL\n";
			registers.HL--;
			cycleAmount = 8;
			break;
		case 0x2C:
			outputFlow << " | INC L\n";
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
			outputFlow << " | DEC L\n";
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
			outputFlow << " | LD L, d8\n";
			registers.L = FetchByte();
			cycleAmount = 8;
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
		case 0x34:
		{
			outputFlow << " | INC (HL)\n";
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
			outputFlow << " | DEC (HL)\n";
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
			outputFlow << " | INC A\n";
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
			outputFlow << " | DEC A\n";
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
			outputFlow << " | LD A, d8\n";
			registers.A = FetchByte();
			cycleAmount = 8;
			break;
		case 0x40:
			outputFlow << " | LD B, B\n";
			cycleAmount = 4;
			break;
		case 0x41:
			outputFlow << " | LD B, C\n";
			registers.B = registers.C;
			cycleAmount = 4;
			break;
		case 0x42:
			outputFlow << " | LD B, D\n";
			registers.B = registers.D;
			cycleAmount = 4;
			break;
		case 0x43:
			outputFlow << " | LD B, E\n";
			registers.B = registers.E;
			cycleAmount = 4;
			break;
		case 0x44:
			outputFlow << " | LD B, H\n";
			registers.B = registers.H;
			cycleAmount = 4;
			break;
		case 0x45:
			outputFlow << " | LD B, L\n";
			registers.B = registers.L;
			cycleAmount = 4;
			break;
		case 0x46:
			outputFlow << " | LD B, (HL)\n";
			registers.B = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x47:
			outputFlow << " | LD C, B\n";
			registers.C = registers.B;
			cycleAmount = 4;
			break;
		case 0x48:
			outputFlow << " | LD C, C\n";
			cycleAmount = 4;
			break;
		case 0x49:
			outputFlow << " | LD C, D\n";
			registers.C = registers.D;
			cycleAmount = 4;
			break;
		case 0x4A:
			outputFlow << " | LD C, E\n";
			registers.C = registers.E;
			cycleAmount = 4;
			break;
		case 0x4B:
			outputFlow << " | LD C, H\n";
			registers.C = registers.H;
			cycleAmount = 4;
			break;
		case 0x4C:
			outputFlow << " | LD C, L\n";
			registers.C = registers.H;
			cycleAmount = 4;
			break;
		case 0x4D:
			outputFlow << " | LD C, L\n";
			registers.C = registers.L;
			cycleAmount = 4;
			break;
		case 0x4E:
			outputFlow << " | LD C, (HL)\n";
			registers.C = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x4F:
			outputFlow << " | LD C, A\n";
			registers.C = registers.A;
			cycleAmount = 4;
			break;
		case 0x50:
			outputFlow << " | LD D, B\n";
			registers.D = registers.B;
			cycleAmount = 4;
			break;
		case 0x51:
			outputFlow << " | LD D, C\n";
			registers.D = registers.C;
			cycleAmount = 4;
			break;
		case 0x52:
			outputFlow << " | LD D, D\n";
			cycleAmount = 4;
			break;
		case 0x53:
			outputFlow << " | LD D, E\n";
			registers.D = registers.E;
			cycleAmount = 4;
			break;
		case 0x54:
			outputFlow << " | LD D, H\n";
			registers.D = registers.H;
			cycleAmount = 4;
			break;
		case 0x55:
			outputFlow << " | LD D, L\n";
			registers.D = registers.L;
			cycleAmount = 4;
			break;
		case 0x56:
			outputFlow << " | LD D, (HL)\n";
			registers.D = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x57:
			outputFlow << " | LD D, A\n";
			registers.D = registers.A;
			cycleAmount = 4;
			break;
		case 0x58:
			outputFlow << " | LD E, B\n";
			registers.E = registers.B;
			cycleAmount = 4;
			break;
		case 0x59:
			outputFlow << " | LD E, C\n";
			registers.E = registers.C;
			cycleAmount = 4;
			break;
		case 0x5A:
			outputFlow << " | LD E, D\n";
			registers.E = registers.D;
			cycleAmount = 4;
			break;
		case 0x5B:
			outputFlow << " | LD E, E\n";
			cycleAmount = 4;
			break;
		case 0x5C:
			outputFlow << " | LD E, H\n";
			registers.E = registers.H;
			cycleAmount = 4;
			break;
		case 0x5D:
			outputFlow << " | LD E, L\n";
			registers.E = registers.L;
			cycleAmount = 4;
			break;
		case 0x5E:
			outputFlow << " | LD E, (HL)\n";
			registers.E = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x5F:
			outputFlow << " | LD E, A\n";
			registers.E = registers.A;
			cycleAmount = 4;
			break;
		case 0x60:
			outputFlow << " | LD H, B\n";
			registers.H = registers.B;
			cycleAmount = 4;
			break;
		case 0x61:
			outputFlow << " | LD H, C\n";
			registers.H = registers.C;
			cycleAmount = 4;
			break;
		case 0x62:
			outputFlow << " | LD H, D\n";
			registers.H = registers.D;
			cycleAmount = 4;
			break;
		case 0x63:
			outputFlow << " | LD H, E\n";
			registers.H = registers.E;
			cycleAmount = 4;
			break;
		case 0x64:
			outputFlow << " | LD H, H\n";
			cycleAmount = 4;
			break;
		case 0x65:
			outputFlow << " | LD H, L\n";
			registers.H = registers.L;
			cycleAmount = 4;
			break;
		case 0x66:
			outputFlow << " | LD H, (HL)\n";
			registers.H = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x67:
			outputFlow << " | LD H, A\n";
			registers.H = registers.A;
			cycleAmount = 4;
			break;
		case 0x68:
			outputFlow << " | LD L, B\n";
			registers.L = registers.B;
			cycleAmount = 4;
			break;
		case 0x69:
			outputFlow << " | LD L, C\n";
			registers.L = registers.C;
			cycleAmount = 4;
			break;
		case 0x6A:
			outputFlow << " | LD L, D\n";
			registers.L = registers.D;
			cycleAmount = 4;
			break;
		case 0x6B:
			outputFlow << " | LD L, E\n";
			registers.L = registers.E;
			cycleAmount = 4;
			break;
		case 0x6C:
			outputFlow << " | LD L, H\n";
			registers.L = registers.H;
			cycleAmount = 4;
			break;
		case 0x6D:
			outputFlow << " | LD L, L\n";
			cycleAmount = 4;
			break;
		case 0x6E:
			outputFlow << " | LD L, (HL)\n";
			registers.L = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x6F:
			outputFlow << " | LD L, A\n";
			registers.L = registers.A;
			cycleAmount = 4;
			break;
		case 0x70:
			outputFlow << " | LD (HL), B\n";
			memory->Write(registers.HL, registers.B);
			cycleAmount = 8;
			break;
		case 0x71:
			outputFlow << " | LD (HL), C\n";
			memory->Write(registers.HL, registers.C);
			cycleAmount = 8;
			break;
		case 0x72:
			outputFlow << " | LD (HL), D\n";
			memory->Write(registers.HL, registers.D);
			cycleAmount = 8;
			break;
		case 0x73:
			outputFlow << " | LD (HL), E\n";
			memory->Write(registers.HL, registers.E);
			cycleAmount = 8;
			break;
		case 0x74:
			outputFlow << " | LD (HL), H\n";
			memory->Write(registers.HL, registers.H);
			cycleAmount = 8;
			break;
		case 0x75:
			outputFlow << " | LD (HL), L\n";
			memory->Write(registers.HL, registers.L);
			cycleAmount = 8;
			break;
		case 0x77:
			outputFlow << " | LD (HL), A\n";
			memory->Write(registers.HL, registers.A);
			cycleAmount = 8;
			break;
		case 0x78:
			outputFlow << " | LD A, B\n";
			registers.A = registers.B;
			cycleAmount = 4;
			break;
		case 0x79:
			outputFlow << " | LD A, C\n";
			registers.A = registers.C;
			cycleAmount = 4;
			break;
		case 0x7A:
			outputFlow << " | LD A, D\n";
			registers.A = registers.D;
			cycleAmount = 4;
			break;
		case 0x7B:
			outputFlow << " | LD A, E\n";
			registers.A = registers.E;
			cycleAmount = 4;
			break;
		case 0x7C:
			outputFlow << " | LD A, H\n";
			registers.A = registers.H;
			cycleAmount = 4;
			break;
		case 0x7D:
			outputFlow << " | LD A, L\n";
			registers.A = registers.L;
			cycleAmount = 4;
			break;
		case 0x7E:
			outputFlow << " | LD A, (HL)\n";
			registers.A = memory->Read(registers.HL);
			cycleAmount = 8;
			break;
		case 0x7F:
			outputFlow << " | LD A, A\n";
			cycleAmount = 4;
			break;
		case 0x80:
		{
			outputFlow << " | ADD A, B\n";
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
			outputFlow << " | ADD A, C\n";
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
			outputFlow << " | ADD A, D\n";
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
			outputFlow << " | ADD A, E\n";
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
			outputFlow << " | ADD A, H\n";
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
			outputFlow << " | ADD A, L\n";
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
			outputFlow << " | ADD A, (HL)\n";
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
			outputFlow << " | ADD A, A\n";
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
			outputFlow << " | ADC A, B\n";
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
			outputFlow << " | ADC A, C\n";
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
			outputFlow << " | ADC A, D\n";
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
			outputFlow << " | ADC A, E\n";
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
			outputFlow << " | ADC A, H\n";
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
			outputFlow << " | ADC A, L\n";
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
			outputFlow << " | ADC A, (HL)\n";
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
			outputFlow << " | ADC A, A\n";
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
			outputFlow << " | SUB B\n";
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
			outputFlow << " | SUB C\n";
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
			outputFlow << " | SUB D\n";
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
			outputFlow << " | SUB E\n";
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
			outputFlow << " | SUB H\n";
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
			outputFlow << " | SUB L\n";
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
			outputFlow << " | SUB (HL)\n";
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
			outputFlow << " | SUB A\n";
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
			outputFlow << " | SBC B\n";
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
			outputFlow << " | SBC C\n";
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
			outputFlow << " | SBC D\n";
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
			outputFlow << " | SBC E\n";
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
			outputFlow << " | SBC H\n";
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
			outputFlow << " | SBC L\n";
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
			outputFlow << " | SBC (HL)\n";
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
			outputFlow << " | SBC A\n";
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
			outputFlow << " | AND B\n";
			registers.A &= registers.B;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA1:
			outputFlow << " | AND C\n";
			registers.A &= registers.C;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA2:
			outputFlow << " | AND D\n";
			registers.A &= registers.D;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA3:
			outputFlow << " | AND E\n";
			registers.A &= registers.E;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA4:
			outputFlow << " | AND H\n";
			registers.A &= registers.H;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA5:
			outputFlow << " | AND L\n";
			registers.A &= registers.L;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA6:
			outputFlow << " | AND (HL)\n";
			registers.A &= memory->Read(registers.HL);
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 8;
			break;
		case 0xA7:
			outputFlow << " | AND A\n";
			registers.A &= registers.A;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(true);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA8:
			outputFlow << " | XOR B\n";
			registers.B = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xA9:
			outputFlow << " | XOR C\n";
			registers.C = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAA:
			outputFlow << " | XOR D\n";
			registers.D = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAB:
			outputFlow << " | XOR E\n";
			registers.E = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAC:
			outputFlow << " | XOR H\n";
			registers.H = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAD:
			outputFlow << " | XOR L\n";
			registers.L = 0;
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xAE:
			outputFlow << " | XOR (HL)\n";
			memory->Write(registers.HL, 0);
			registers.SetZeroFlag(true);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
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
		case 0xB0:
			outputFlow << " | OR B\n";
			registers.A |= registers.B;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB1:
			outputFlow << " | OR C\n";
			registers.A |= registers.C;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB2:
			outputFlow << " | OR D\n";
			registers.A |= registers.D;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB3:
			outputFlow << " | OR E\n";
			registers.A |= registers.E;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB4:
			outputFlow << " | OR H\n";
			registers.A |= registers.H;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB5:
			outputFlow << " | OR L\n";
			registers.A |= registers.L;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB6:
			outputFlow << " | OR (HL)\n";
			registers.A |= memory->Read(registers.HL);
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 8;
			break;
		case 0xB7:
			outputFlow << " | OR A\n";
			registers.A |= registers.A;
			registers.SetZeroFlag(registers.A == 0);
			registers.SetSubtractFlag(false);
			registers.SetHalfCarryFlag(false);
			registers.SetCarryFlag(false);
			cycleAmount = 4;
			break;
		case 0xB8:
			outputFlow << " | CP B\n";
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
			outputFlow << " | CP C\n";
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
			outputFlow << " | CP D\n";
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
			outputFlow << " | CP E\n";
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
			outputFlow << " | CP H\n";
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
			outputFlow << " | CP H\n";
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
			outputFlow << " | CP (HL)\n";
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
			outputFlow << " | CP A\n";
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
			outputFlow << " | POP BC\n";
			registers.B = memory->Read(SP++);
			registers.C = memory->Read(SP++);
			cycleAmount = 12;
			break;
		case 0xC5:
			outputFlow << " | PUSH BC\n";
			memory->Write(--SP, registers.C);
			memory->Write(--SP, registers.B);
			cycleAmount = 24;
			break;
		case 0xC9:
		{
			outputFlow << " | RET\n";
			u8 low = memory->Read(SP++);
			u8 high = memory->Read(SP++);
			PC = (high << 8) | low;
			cycleAmount = 16;
			break;
		}
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
		case 0xEA:
		{
			outputFlow << " | LD a16, A\n";
			u16 address = FetchWord();
			memory->Write(address, registers.A);
			cycleAmount = 16;
			break;
		}
		case 0xF0:
		{
			outputFlow << " | LD A, (a8)\n";
			u8 offset = FetchByte();
			registers.A = memory->Read(0xFF00 + offset);
			cycleAmount = 12;
			break;
		}
		case 0xFE:
		{
			outputFlow << " | CP d8\n";
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
	if (PC == 0x100)
	{
		std::printf("Boot Rom finished");
	}

	OP = FetchByte();
	outputFlow << "Instruction: " << std::format("{:#04X}", OP);

	return Decode();
}
