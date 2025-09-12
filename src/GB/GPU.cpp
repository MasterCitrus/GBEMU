#include "GPU.h"

GPU::GPU()
	: vram(0x2000, 0), oam(0xA0, 0), framebuffer(160 * 144 * 3, 0),
	lcdc(0), stat(0), scy(0), scx(0), ly(0), lyc(0), wy(0), wx(0),
	bgp(0), obp0(0), obp1(0), scanlineCycles(0), memory(memory)
{
}

void GPU::Step(int cycles)
{
	scanlineCycles += cycles;

	if (scanlineCycles >= 456)
	{
		scanlineCycles -= 456;
		ly++;

		if (ly == 144)
		{
			stat |= 0x01;
		}
		else if (ly > 153)
		{
			ly = 0;
			stat &= 0xFC;
		}
	}
}

u8 GPU::ReadVRAM(u16 address)
{
	return vram[address - 0x8000];
}

void GPU::WriteVRAM(u16 address, u8 value)
{
	vram[address - 0x8000] = value;
}

u8 GPU::ReadOAM(u16 address)
{
	return oam[address];
}

void GPU::WriteOAM(u16 address, u8 value)
{
	oam[address - 0xFE00] = value;
}

u8 GPU::ReadRegister(u16 address)
{
	switch (address)
	{
		case 0xFF40:
			return lcdc;
			break;
		case 0xFF41:
			return stat;
			break;
		case 0xFF42:
			return scy;
			break;
		case 0xFF43:
			return scx;
			break;
		case 0xFF44:
			return ly;
			break;
		case 0xFF45:
			return lyc;
			break;
		case 0xFF46:
			break;
		case 0xFF47:
			return bgp;
			break;
		case 0xFF48:
			return obp0;
			break;
		case 0xFF49:
			return obp1;
			break;
		case 0xFF4A:
			return wy;
			break;
		case 0xFF4B:
			return wx;
			break;
	}
}

void GPU::SetRegister(u16 address, u8 value)
{
	switch (address)
	{
		case 0xFF40:
			lcdc = value;
			break;
		case 0xFF41:
			stat = value;
			break;
		case 0xFF42:
			scy = value;
			break;
		case 0xFF43:
			scx = value;
			break;
		case 0xFF44:
			break;
		case 0xFF45:
			lyc = value;
			break;
		case 0xFF46:
			break;
		case 0xFF47:
			bgp = value;
			break;
		case 0xFF48:
			obp0 = value;
			break;
		case 0xFF49:
			obp1 = value;
			break;
		case 0xFF4A:
			wy = value;
			break;
		case 0xFF4B:
			wx = value;
			break;
	}
}
