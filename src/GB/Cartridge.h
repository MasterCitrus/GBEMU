#pragma once

#include "Types.h"
#include <string>
#include <vector>

class Cartridge
{
public:
	Cartridge();

	bool LoadCart(const std::string& cartFilePath);

	u8 ReadRom(u16 address) const;
	void WriteRom(u16 address, u8 value);

	u8 ReadRAM(u16 address) const;
	void WriteRAM(u16 address, u8 value);

	std::string GetTitle() const { return cartTitle; }

private:
	std::vector<u8> rom;
	std::vector<u8> ram;
	std::string cartTitle;
	u8 mbcType;
	int romBanks;
	int ramBanks;
	int currentRomBank;
	int currentRamBank;
	bool ramEnabled;
};