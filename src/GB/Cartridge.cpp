#include "Cartridge.h"
#include <fstream>
#include <iostream>

Cartridge::Cartridge()
	: mbcType(0), romBanks(0), ramBanks(0), currentRomBank(1), currentRamBank(0), ramEnabled(false)
{
}

bool Cartridge::LoadCart(const std::string& cartFilePath)
{
	std::ifstream cart(cartFilePath, std::ios::binary | std::ios::ate);
	if (!cart)
	{
		return false;
	}

	size_t cartSize = cart.tellg();
	cart.seekg(0, std::ios::beg);

	rom.resize(cartSize);
	cart.read(reinterpret_cast<char*>(rom.data()), cartSize);

	if (cartSize >= 0x150)
	{
		for (int i = 0x134; i < 0x143 && rom[i] != 0; i++)
		{
			cartTitle += static_cast<char>(rom[i]);
		}

		mbcType = rom[0x147];

		u8 romSizeCode = rom[0x148];
		romBanks = 2 << romSizeCode;

		u8 ramSizeCode = rom[0x149];
		switch (ramSizeCode)
		{
			case 0:
				ramBanks = 0;
				break;
			case 1:
			case 2:
				ramBanks = 2;
				break;
			case 3:
				ramBanks = 4;
				break;
			case 4:
				ramBanks = 16;
				break;
			case 5:
				ramBanks = 8;
				break;
		}

		if (ramBanks > 0)
		{
			ram.resize(ramBanks * 2000, 0);
		}
	}

	std::cout << "Current ROM: " << cartTitle << "\n\n";

	return true;
}

u8 Cartridge::ReadRom(u16 address) const
{
	if (address < 0x4000)
	{
		if (address < rom.size())
		{
			return rom[address];
		}
	}
	else if (address < 0x8000)
	{
		int bankAddress = (currentRomBank * 0x4000) + (address - 0x4000);
		if (bankAddress < rom.size())
		{
			return rom[bankAddress];
		}
	}

	return 0xFF;
}

void Cartridge::WriteRom(u16 address, u8 value)
{
	if (mbcType == 0) return;

	if (address < 0x2000)
	{
		ramEnabled = (value & 0xFF) == 0x0A;
	}
	else if (address < 0x4000)
	{
		currentRomBank = value & 0x1F;
		if (currentRomBank == 0) currentRomBank = 1;
	}
	else if (address < 0x6000)
	{
		currentRamBank = value & 0x03;
	}
}

u8 Cartridge::ReadRAM(u16 address) const
{
	if (!ramEnabled || ram.empty())
	{
		return 0xFF;
	}

	int ramAddress = (currentRamBank * 0x2000) + (address - 0xA000);
	if (ramAddress < ram.size())
	{
		return ram[ramAddress];
	}

	return 0xFF;
}

void Cartridge::WriteRAM(u16 address, u8 value)
{
	if (!ramEnabled || ram.empty()) return;

	int ramAddress = (currentRamBank * 0x2000) + (address - 0xA000);
	if (ramAddress < ram.size())
	{
		ram[ramAddress] = value;
	}
}
