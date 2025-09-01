#include "CPU.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

CPU::CPU()
{
	std::ifstream bootRom;
	bootRom.open("BootRoms/dmg_boot.bin", std::ios::binary | std::ios::ate);

	std::streamsize size = bootRom.tellg();
	bootRom.seekg(0, std::ios::beg);

	bootRom.read(reinterpret_cast<char*>(memory), size);
}

int CPU::Step()
{
	u8 opcode = memory[registers.PC];
	u16 programCode = registers.PC;

	switch (memory[programCode])
	{
		// ROW 0
		case 0x00:
			std::printf("NOP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.PC++;
			return 1;
			break;
		case 0x01:
		{
			std::printf("LD BC, d16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 low = memory[registers.PC++];
			u8 high = memory[registers.PC++];
			registers.BC = static_cast<u16>(low);
			registers.BC |= static_cast<u16>(high) << 8;
			registers.PC++;
			return 1;
			break;
		}
		case 0x02:
			std::printf("LD (BC), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x03:
			std::printf("INC BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			
			return 0;
			break;
		case 0x04:
		{
			std::printf("INC B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 result = registers.B + 1;
			registers.F &= 0x10;
			if (result == 0)
			{
				registers.F = 0x80;
			}
			if ((registers.B & 0x0F) + 1 > 0x0F)
			{
				registers.F |= 0x20;
			}
			registers.B = result;
			registers.PC++;
			return 0;
			break;
		}
		case 0x05:
			std::printf("DEC BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 1;
			break;
		case 0x06:
			std::printf("LD B, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x07:
			std::printf("RLCA | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x08:
			std::printf("LD (a16), SP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x09:
			std::printf("ADD HL, BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x0A:
			std::printf("LD A, BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.A = memory[registers.BC];
			registers.PC++;
			return 1;
			break;
		case 0x0B:
			std::printf("DEC BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x0C:
			std::printf("INC C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x0D:
			std::printf("DEC C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x0E:
			std::printf("LD C, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x0F:
			std::printf("RRCA | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 1
		case 0x10:
			std::printf("STOP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x11:
			std::printf("LD DE, d16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x12:
			std::printf("LD (DE), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x13:
			std::printf("INC DE | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x14:
			std::printf("INC D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x15:
			std::printf("DEC D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x16:
			std::printf("LD D, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x17:
			std::printf("RLA | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x18:
			std::printf("JR s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x19:
			std::printf("ADD HL, DE | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x1A:
			std::printf("LD A, (DE) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x1B:
			std::printf("DEC DE | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x1C:
			std::printf("INC E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x1D:
			std::printf("DEC E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x1E:
			std::printf("LD E, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.E = memory[registers.PC++];
			registers.PC++;
			return 1;
			break;
		case 0x1F:
			std::printf("RRA | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 2
		case 0x20:
			std::printf("JR NZ, s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			if ((registers.F & 0x80) == 0)
			{
				registers.PC = registers.PC + (s8)memory[registers.PC];
			}
			else
			{
				registers.PC++;
			}
			return 1;
			break;
		case 0x21:
		{
			std::printf("LD HL, d16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 low = memory[registers.PC++];
			u8 high = memory[registers.PC++];
			registers.HL = static_cast<u16>(low);
			registers.HL |= static_cast<u16>(high) << 8;
			registers.PC++;
			return 1;
			break;
		}
		case 0x22:
			std::printf("LD (HL+), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x23:
			std::printf("INC HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x24:
			std::printf("INC H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x25:
			std::printf("DEC H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x26:
			std::printf("LD H, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x27:
			std::printf("DAA | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x28:
			std::printf("JR Z, s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x29:
			std::printf("ADD HL, HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2A:
			std::printf("LD A, (HL+) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2B:
			std::printf("DEC HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2C:
			std::printf("INC L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2D:
			std::printf("DEC L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2E:
			std::printf("LD L, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x2F:
			std::printf("CPL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 3
		case 0x30:
			std::printf("JR NC, s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x31:
		{
			std::printf("LD SP, d16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 low = memory[registers.PC++];
			u8 high = memory[registers.PC++];
			registers.SP = static_cast<u16>(low);
			registers.SP |= static_cast<u16>(high) << 8;
			registers.PC++;
			return 1;
			break;
		}
		case 0x32:
			std::printf("LD (HL-), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL--] = registers.A;
			registers.PC++;
			return 1;
			break;
		case 0x33:
			std::printf("INC SP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 1;
			break;
		case 0x34:
		{
			std::printf("INC (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 val = memory[registers.HL];
			val++;
			memory[registers.HL] = val;
			registers.F &= 0x10;
			if (val == 0)
			{
				registers.F |= 0x80;
			}
			if ((val & 0x0F) == 0x0F)
			{
				registers.F |= 0x20;
			}
			registers.PC++;
			return 1;
			break;
		}
		case 0x35:
			std::printf("DEC (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x36:
			std::printf("LD (HL), d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x37:
			std::printf("SCF | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x38:
			std::printf("JR C, s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x39:
			std::printf("ADD HL, SP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3A:
			std::printf("LD A, (HL-) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3B:
			std::printf("DEC SP | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3C:
			std::printf("INC A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3D:
			std::printf("DEC A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3E:
			std::printf("LD A, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x3F:
			std::printf("CCF | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 4
		case 0x40:
			std::printf("LD B, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x41:
			std::printf("LD B, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x42:
			std::printf("LD B, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x43:
			std::printf("LD B, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x44:
			std::printf("LD B, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x45:
			std::printf("LD B, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x46:
			std::printf("LD B, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x47:
			std::printf("LD B, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x48:
			std::printf("LD C, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x49:
			std::printf("LD C, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4A:
			std::printf("LD C, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4B:
			std::printf("LD C, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4C:
			std::printf("LD C, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4D:
			std::printf("LD C, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4E:
			std::printf("LD C, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x4F:
			std::printf("LD C, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 5
		case 0x50:
			std::printf("LD D, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x51:
			std::printf("LD D, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x52:
			std::printf("LD D, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x53:
			std::printf("LD D, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x54:
			std::printf("LD D, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x55:
			std::printf("LD D, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x56:
			std::printf("LD D, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x57:
			std::printf("LD D, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x58:
			std::printf("LD E, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x59:
			std::printf("LD E, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5A:
			std::printf("LD E, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5B:
			std::printf("LD E, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5C:
			std::printf("LD E, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5D:
			std::printf("LD E, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5E:
			std::printf("LD E, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x5F:
			std::printf("LD E, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 6
		case 0x60:
			std::printf("LD H, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 1;
			break;
		case 0x61:
			std::printf("LD H, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x62:
			std::printf("LD H, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x63:
			std::printf("LD H, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x64:
			std::printf("LD H, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x65:
			std::printf("LD H, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x66:
			std::printf("LD H, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x67:
			std::printf("LD H, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x68:
			std::printf("LD L, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x69:
			std::printf("LD L, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6A:
			std::printf("LD L, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6B:
			std::printf("LD L, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6C:
			std::printf("LD L, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6D:
			std::printf("LD L, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6E:
			std::printf("LD L, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x6F:
			std::printf("LD L, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 7
		case 0x70:
			std::printf("LD (HL), B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.B;
			registers.PC++;
			return 1;
			break;
		case 0x71:
			std::printf("LD (HL), C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.C;
			registers.PC++;
			return 1;
			break;
		case 0x72:
			std::printf("LD (HL), D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.D;
			registers.PC++;
			return 1;
			break;
		case 0x73:
			std::printf("LD (HL), E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.E;
			registers.PC++;
			return 1;
			break;
		case 0x74:
			std::printf("LD (HL), H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.H;
			registers.PC++;
			return 1;
			break;
		case 0x75:
			std::printf("LD (HL), L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.L;
			registers.PC++;
			return 1;
			break;
		case 0x76:
			std::printf("HALT | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x77:
			std::printf("LD (HL), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			memory[registers.HL] = registers.A;
			registers.PC++;
			return 1;
			break;
		case 0x78:
			std::printf("LD A, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x79:
			std::printf("LD A, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x7A:
			std::printf("LD A, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x7B:
			std::printf("LD A, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x7C:
			std::printf("LD A, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.A = registers.H;
			registers.PC++;
			return 1;
			break;
		case 0x7D:
			std::printf("LD A, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x7E:
			std::printf("LD A, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x7F:
			std::printf("LD A, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 8
		case 0x80:
			std::printf("ADD A, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.F = 0;
			if (registers.A == 0)
			{
				registers.F |= 0x80;
			}
			if (((registers.A & 0x0F) + (registers.B & 0x0F)) > 0x0F)
			{
				registers.F |= 0x20;
			}
			if ((registers.A + registers.B) > 0xFF)
			{
				registers.F |= 0x10;
			}
			registers.A = registers.A + registers.B;
			registers.PC++;
			return 1;
			break;
		case 0x81:
			std::printf("ADD A, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x82:
			std::printf("ADD A, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x83:
			std::printf("ADD A, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x84:
			std::printf("ADD A, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x85:
			std::printf("ADD A, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x86:
			std::printf("ADD A, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x87:
			std::printf("ADD A, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x88:
			std::printf("ADC A, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x89:
			std::printf("ADC A, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8A:
			std::printf("ADC A, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8B:
			std::printf("ADC A, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8C:
			std::printf("ADC A, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8D:
			std::printf("ADC A, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8E:
			std::printf("ADC A, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x8F:
			std::printf("ADC A, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 9
		case 0x90:
			std::printf("SUB B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x91:
			std::printf("SUB C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x92:
			std::printf("SUB D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x93:
			std::printf("SUB E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x94:
			std::printf("SUB H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x95:
			std::printf("SUB L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x96:
			std::printf("SUB (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x97:
			std::printf("SUB A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x98:
			std::printf("SBC A, B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x99:
			std::printf("SBC A, C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9A:
			std::printf("SBC A, D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9B:
			std::printf("SBC A, E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9C:
			std::printf("SBC A, H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9D:
			std::printf("SBC A, L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9E:
			std::printf("SBC A, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0x9F:
			std::printf("SBC A, A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 10
		case 0xA0:
			std::printf("AND B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA1:
			std::printf("AND C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA2:
			std::printf("AND D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA3:
			std::printf("AND E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA4:
			std::printf("AND H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA5:
			std::printf("AND L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA6:
			std::printf("AND (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA7:
			std::printf("AND A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA8:
			std::printf("XOR B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xA9:
			std::printf("XOR C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAA:
			std::printf("XOR D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAB:
			std::printf("XOR E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAC:
			std::printf("XOR H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAD:
			std::printf("XOR L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAE:
			std::printf("XOR (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xAF:
			std::printf("XOR A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.AF = registers.AF ^ registers.AF;
			registers.AF ^= (1u << 15);
			registers.PC++;
			return 1;
			break;
		// ROW 11
		case 0xB0:
			std::printf("OR B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB1:
			std::printf("OR C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB2:
			std::printf("OR D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB3:
			std::printf("OR E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB4:
			std::printf("OR H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB5:
			std::printf("OR L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB6:
			std::printf("OR (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB7:
			std::printf("OR A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB8:
			std::printf("CP B | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xB9:
			std::printf("CP C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xBA:
			std::printf("CP D | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xBB:
			std::printf("CP E | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xBC:
			std::printf("CP H | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xBD:
			std::printf("CP L | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xBE:
		{
			std::printf("CP (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 temp = registers.A - memory[registers.HL];
			registers.F = 0;
			if (temp == 0)
			{
				registers.F |= 0x80;
			}
			registers.F |= 0x40;
			if ((registers.A & 0x0F) < (memory[registers.HL] & 0x0F))
			{
				registers.F |= 0x20;
			}
			if (registers.A < memory[registers.HL])
			{
				registers.F |= 0x10;
			}
			registers.PC++;
			return 1;
			break;
		}
		case 0xBF:
			std::printf("CP A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 12
		case 0xC0:
			std::printf("RET NZ | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC1:
			std::printf("POP BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC2:
			std::printf("JP NZ, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC3:
			std::printf("JP a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC4:
			std::printf("CALL NZ, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC5:
			std::printf("PUSH BC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC6:
			std::printf("ADD A, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC7:
			std::printf("RST 0 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC8:
			std::printf("RET Z | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xC9:
			std::printf("RET | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xCA:
			std::printf("JP Z, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xCB:
			std::printf("PREFIX | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			registers.PC++;
			return PrefixStep(registers.PC);
			break;
		case 0xCC:
			std::printf("CALL Z, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xCD:
		{
			std::printf("CALL a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 low = memory[registers.PC++];
			u8 high = memory[registers.PC++];
			u16 target = static_cast<u16>(low);
			target |= static_cast<u16>(high) << 8;
			memory[registers.SP - 1] = (registers.PC >> 8) & 0xFF;
			memory[registers.SP - 2] = registers.PC & 0xFF;
			registers.SP -= 2;
			registers.PC = target;
			return 1;
			break;
		}
		case 0xCE:
			std::printf("ADC A, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xCF:
			std::printf("RST 1 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 13
		case 0xD0:
			std::printf("RET NC | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD1:
			std::printf("POP DE | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD2:
			std::printf("JP NC, a16	 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD3:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD4:
			std::printf("CALL NC, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD5:
			std::printf("PUSH DE | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD6:
			std::printf("SUB d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD7:
			std::printf("RST 2 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD8:
			std::printf("RET C | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xD9:
			std::printf("RETI | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDA:
			std::printf("JP C, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDB:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDC:
			std::printf("CALL C, a16 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDD:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDE:
			std::printf("SBC A, d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xDF:
			std::printf("RST 3 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 14
		case 0xE0:
			std::printf("LD (a8), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE1:
			std::printf("POP HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE2:
			std::printf("LD (C), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE3:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE4:
			std::printf("UNSUSED | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE5:
			std::printf("PUSH HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE6:
			std::printf("AND d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE7:
			std::printf("RST 4 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE8:
			std::printf("ADD SP, s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xE9:
			std::printf("JP HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xEA:
			std::printf("LD (A16), A | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xEB:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xEC:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xED:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xEE:
			std::printf("XOR d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xEF:
			std::printf("RST 5 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		// ROW 15
		case 0xF0:
			std::printf("LD A, (a8) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF1:
			std::printf("POP AF | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF2:
			std::printf("LD A, (C) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF3:
			std::printf("DI | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF4:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF5:
			std::printf("PUSH AF | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF6:
			std::printf("OR d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF7:
			std::printf("RST 6 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF8:
			std::printf("LD HL, SP+s8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xF9:
			std::printf("LD SP, HL | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xFA:
			std::printf("LD A, (a16) | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xFB:
			std::printf("EI | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xFC:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xFD:
			std::printf("INVALID | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			return 0;
			break;
		case 0xFE:
		{
			std::printf("CP d8 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			u8 immediateVal = memory[registers.PC++];
			u8 temp = registers.A - immediateVal;
			registers.F = 0;
			if (temp == 0)
			{
				registers.F |= 0x80;
			}
			registers.F |= 0x40;
			if ((registers.A & 0x0F) < (immediateVal & 0x0F))
			{
				registers.F |= 0x20;
			}
			if (registers.A < immediateVal)
			{
				registers.F |= 0x10;
			}
			registers.PC++;
			return 1;
			break;
		}
		case 0xFF:
		{
			std::printf("RST 7 | Op Code: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			//u16 returnAddr = registers.PC;

			//registers.SP -= 2;
			//memory[registers.SP] = returnAddr & 0xFF;
			//memory[registers.SP + 1] = (returnAddr >> 8) & 0xFF;
			return 0;
			break;
		}
		default:
			std::printf("Unsupported opcode: 0x%02x at 0x%04x\n\n\n", opcode, programCode);
			std::exit(EXIT_FAILURE);
			registers.PC++;
			return 0;
			break;
	}
}

int CPU::PrefixStep(u16 programCode)
{
	u8 prefixOpcode = memory[programCode];

	switch (prefixOpcode)
	{
		// ROW 0
		case 0x00:
			std::printf("RLC B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x01:
		{
			std::printf("RLC C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		}
		case 0x02:
			std::printf("RLC D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x03:
			std::printf("RLC E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x04:
			std::printf("RLC H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x05:
			std::printf("RLC L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x06:
			std::printf("RLC (HL) | Op Code : 0x % 02x at 0x % 04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x07:
			std::printf("RLC A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x08:
			std::printf("RRC B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x09:
			std::printf("RRC C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0A:
			std::printf("RRC D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0B:
			std::printf("RRC E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0C:
			std::printf("RRC H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0D:
			std::printf("RRC L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0E:
			std::printf("RRC (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x0F:
			std::printf("RRC A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 1
		case 0x10:
			std::printf("RL B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x11:
			std::printf("RL C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x12:
			std::printf("RL D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x13:
			std::printf("RL E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x14:
			std::printf("RL H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x15:
			std::printf("RL L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x16:
			std::printf("RL (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x17:
			std::printf("RL A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x18:
			std::printf("RR B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x19:
			std::printf("RR C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1A:
			std::printf("RR D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1B:
			std::printf("RR E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1C:
			std::printf("RR H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1D:
			std::printf("RR L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1E:
			std::printf("RR (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x1F:
			std::printf("RR A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 2
		case 0x20:
			std::printf("SLA B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x21:
		{
			std::printf("SLA C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		}
		case 0x22:
			std::printf("SLA D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x23:
			std::printf("SLA E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x24:
			std::printf("SLA H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x25:
			std::printf("SLA L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x26:
			std::printf("SLA (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x27:
			std::printf("SLA A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x28:
			std::printf("SRA B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x29:
			std::printf("SRA C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2A:
			std::printf("SRA D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2B:
			std::printf("SRA E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2C:
			std::printf("SRA H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2D:
			std::printf("SRA L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2E:
			std::printf("SRA (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x2F:
			std::printf("SRA A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 3
		case 0x30:
			std::printf("SWAP B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x31:
		{
			std::printf("SWAP C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		}
		case 0x32:
			std::printf("SWAP D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x33:
			std::printf("SWAP E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x34:
			std::printf("SWAP H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x35:
			std::printf("SWAP L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x36:
			std::printf("SWAP (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x37:
			std::printf("SWAP A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x38:
			std::printf("SRL B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x39:
			std::printf("SRL C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3A:
			std::printf("SRL D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3B:
			std::printf("SRL E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3C:
			std::printf("SRL H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3D:
			std::printf("SRL L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3E:
			std::printf("SRL (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x3F:
			std::printf("SRL A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 4
		case 0x40:
			std::printf("BIT 0, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x41:
			std::printf("BIT 0, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x42:
			std::printf("BIT 0, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x43:
			std::printf("BIT 0, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x44:
			std::printf("BIT 0, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x45:
			std::printf("BIT 0, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x46:
			std::printf("BIT 0, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x47:
			std::printf("BIT 0, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x48:
			std::printf("BIT 1, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x49:
			std::printf("BIT 1, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4A:
			std::printf("BIT 1, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4B:
			std::printf("BIT 1, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4C:
			std::printf("BIT 1, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4D:
			std::printf("BIT 1, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4E:
			std::printf("BIT 1, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x4F:
			std::printf("BIT 1, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 5
		case 0x50:
			std::printf("BIT 2, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x51:
			std::printf("BIT 2, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x52:
			std::printf("BIT 2, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x53:
			std::printf("BIT 2, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x54:
			std::printf("BIT 2, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x55:
			std::printf("BIT 2, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x56:
			std::printf("BIT 2, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x57:
			std::printf("BIT 2, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x58:
			std::printf("BIT 3, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x59:
			std::printf("BIT 3, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5A:
			std::printf("BIT 3, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5B:
			std::printf("BIT 3, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5C:
			std::printf("BIT 3, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5D:
			std::printf("BIT 3, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5E:
			std::printf("BIT 3, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x5F:
			std::printf("BIT 3, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 6
		case 0x60:
			std::printf("BIT 4, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x61:
			std::printf("BIT 4, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x62:
			std::printf("BIT 4, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x63:
			std::printf("BIT 4, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x64:
			std::printf("BIT 4, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x65:
			std::printf("BIT 4, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x66:
			std::printf("BIT 4, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x67:
			std::printf("BIT 4, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x68:
			std::printf("BIT 5, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x69:
			std::printf("BIT 5, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6A:
			std::printf("BIT 5, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6B:
			std::printf("BIT 5, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6C:
			std::printf("BIT 5, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6D:
			std::printf("BIT 5, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6E:
			std::printf("BIT 5, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x6F:
			std::printf("BIT 5, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 7
		case 0x70:
			std::printf("BIT 6, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x71:
			std::printf("BIT 6, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x72:
			std::printf("BIT 6, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x73:
			std::printf("BIT 6, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x74:
			std::printf("BIT 6, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x75:
			std::printf("BIT 6, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x76:
			std::printf("BIT 6 | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x77:
			std::printf("BIT 6, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x78:
			std::printf("BIT 7, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x79:
			std::printf("BIT 7, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x7A:
			std::printf("BIT 7, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x7B:
			std::printf("BIT 7, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x7C:
		{
			std::printf("BIT 7, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			bool bit = (registers.H & 0x80) == 0;
			registers.F = 0;
			registers.F |= 0x10;
			if (bit)
			{
				registers.F |= 0x80;
			}
			return 1;
			break;
		}
		case 0x7D:
			std::printf("BIT 7, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x7E:
			std::printf("BIT 7, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x7F:
			std::printf("BIT 7, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 8
		case 0x80:
			std::printf("RES 0, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x81:
			std::printf("RES 0, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x82:
			std::printf("RES 0, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x83:
			std::printf("RES 0, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x84:
			std::printf("RES 0, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x85:
			std::printf("RES 0, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x86:
			std::printf("RES 0, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x87:
			std::printf("RES 0, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x88:
			std::printf("RES 1, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x89:
			std::printf("RES 1, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8A:
			std::printf("RES 1, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8B:
			std::printf("RES 1, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8C:
			std::printf("RES 1, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8D:
			std::printf("RES 1, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8E:
			std::printf("RES 1, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x8F:
			std::printf("RES 1, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 9
		case 0x90:
			std::printf("RES 2, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x91:
			std::printf("RES 2, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x92:
			std::printf("RES 2, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x93:
			std::printf("RES 2, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x94:
			std::printf("RES 2, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x95:
			std::printf("RES 2, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x96:
			std::printf("RES 2, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x97:
			std::printf("RES 2, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x98:
			std::printf("RES 3, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x99:
			std::printf("RES 3, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9A:
			std::printf("RES 3, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9B:
			std::printf("RES 3, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9C:
			std::printf("RES 3, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9D:
			std::printf("RES 3, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9E:
			std::printf("RES 3, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0x9F:
			std::printf("RES 3, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 10
		case 0xA0:
			std::printf("RES 4, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA1:
			std::printf("RES 4, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA2:
			std::printf("RES 4, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA3:
			std::printf("RES 4, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA4:
			std::printf("RES 4, H | Op Code : 0x % 02x at 0x % 04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA5:
			std::printf("RES 4, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA6:
			std::printf("RES 4, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA7:
			std::printf("RES 4, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA8:
			std::printf("RES 5, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xA9:
			std::printf("RES 5, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAA:
			std::printf("RES 5, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAB:
			std::printf("RES 5, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAC:
			std::printf("RES 5, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAD:
			std::printf("RES 5, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAE:
			std::printf("RES 5, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xAF:
			std::printf("RES 5, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 11
		case 0xB0:
			std::printf("RES 6, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB1:
			std::printf("RES 6, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB2:
			std::printf("RES 6, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB3:
			std::printf("RES 6, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB4:
			std::printf("RES 6, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB5:
			std::printf("RES 6, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB6:
			std::printf("RES 6, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB7:
			std::printf("RES 6, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB8:
			std::printf("RES 7, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xB9:
			std::printf("RES 7, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBA:
			std::printf("RES 7, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBB:
			std::printf("RES 7, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBC:
			std::printf("RES 7, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBD:
			std::printf("RES 7, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBE:
			std::printf("RES 7, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xBF:
			std::printf("RES 7, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 12
		case 0xC0:
			std::printf("SET 0, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC1:
			std::printf("SET 0, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC2:
			std::printf("SET 0, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC3:
			std::printf("SET 0, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC4:
			std::printf("SET 0, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC5:
			std::printf("SET 0, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC6:
			std::printf("SET 0, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC7:
			std::printf("SET 0, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC8:
			std::printf("SET 1, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xC9:
			std::printf("SET 1, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCA:
			std::printf("SET 1, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCB:
			std::printf("SET 1, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCC:
			std::printf("SET 1, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCD:
			std::printf("SET 1, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCE:
			std::printf("SET 1, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xCF:
			std::printf("SET 1, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 13
		case 0xD0:
			std::printf("SET 2, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD1:
			std::printf("SET 2, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD2:
			std::printf("SET 2, D	 | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD3:
			std::printf("SET 2, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD4:
			std::printf("SET 2, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD5:
			std::printf("SET 2, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD6:
			std::printf("SET 2, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD7:
			std::printf("SET 2, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD8:
			std::printf("SET 3, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xD9:
			std::printf("SET 3, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDA:
			std::printf("SET 3, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDB:
			std::printf("SET 3, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDC:
			std::printf("SET 3, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDD:
			std::printf("SET 3, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDE:
			std::printf("SET 3, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xDF:
			std::printf("SET 3, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 14
		case 0xE0:
			std::printf("SET 4, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE1:
			std::printf("SET 4, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE2:
			std::printf("SET 4, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE3:
			std::printf("SET 4, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE4:
			std::printf("SET 4, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE5:
			std::printf("SET 4, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE6:
			std::printf("SET 4, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE7:
			std::printf("SET 4, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE8:
			std::printf("SET 5, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xE9:
			std::printf("SET 5, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xEA:
			std::printf("SET 5, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xEB:
			std::printf("SET 5, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xEC:
			std::printf("SET 5, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xED:
			std::printf("SET 5, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xEE:
			std::printf("SET 5, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xEF:
			std::printf("SET 5, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
			// ROW 15
		case 0xF0:
			std::printf("SET 6, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF1:
			std::printf("SET 6, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF2:
			std::printf("SET 6, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF3:
			std::printf("SET 6, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF4:
			std::printf("SET 6, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF5:
			std::printf("SET 6, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF6:
			std::printf("SET 6, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF7:
			std::printf("SET 6, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF8:
			std::printf("SET 7, B | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xF9:
			std::printf("SET 7, C | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFA:
			std::printf("SET 7, D | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFB:
			std::printf("SET 7, E | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFC:
			std::printf("SET 7, H | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFD:
			std::printf("SET 7, L | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFE:
			std::printf("SET 7, (HL) | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		case 0xFF:
		{
			std::printf("SET 7, A | Op Code: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			return 1;
			break;
		}
		default:
			std::printf("Unsupported prefix opcode: 0x%02x at 0x%04x\n\n\n", prefixOpcode, programCode);
			std::exit(EXIT_FAILURE);
			registers.PC++;
			return 0;
			break;
	}
}
