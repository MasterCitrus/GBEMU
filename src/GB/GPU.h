#pragma once
#include "Types.h"
#include <vector>

class Memory;

class GPU
{
public:
	GPU();

	void Step(int cycles);

	u8 ReadVRAM(u16 address);
	void WriteVRAM(u16 address, u8 value);

	u8 ReadOAM(u16 address);
	void WriteOAM(u16 address, u8 value);

	u8 ReadRegister(u16 address);
	void SetRegister(u16 address, u8 value);

	const std::vector<u8> GetFramebuffer() const { return framebuffer; }

private:
	std::vector<u8> vram;
	std::vector<u8> oam;
	std::vector<u8> framebuffer;

	Memory* memory;

	u8 lcdc;
	u8 stat;
	u8 scy;
	u8 scx;
	u8 ly;
	u8 lyc;
	u8 wy;
	u8 wx;
	u8 bgp;
	u8 obp0;
	u8 obp1;

	int scanlineCycles;
};