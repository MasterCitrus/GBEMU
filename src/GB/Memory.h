#pragma once

#include "Types.h"

#include <fstream>
#include <string>
#include <vector>

enum class MemoryArea
{
	INVALID,
	BOOTROM,
	ROM,
	VIDEO_RAM,
	EXTERNAL_RAM,
	WORK_RAM,
	ECHO_RAM,
	OAM,
	PROHIBITIED,
	IO_REGISTERS,
	HIGH_RAM,
	INTERRUPT_ENABLE_REGISTER
};

class Cartridge;

class Memory
{
public:
	Memory();
	~Memory();

	u8 Read(u16 address) const;
	void Write(u16 address, u8 value);

	bool LoadBootRom(const std::string& bootRomPath);
	void SetCart(Cartridge* cart);

	void DisableBootRom() { bootRomEnabled = false; }
	bool BootRomEnabled() { return bootRomEnabled; }

private:
	constexpr MemoryArea GetMemoryArea(const u16 address) const;

private:
	std::vector<u8> bootRom;
	std::vector<u8> workRam;
	std::vector<u8> highRam;
	std::vector<u8> ioRegisters;
	std::ofstream output;
	Cartridge* cart;
	bool bootRomEnabled;
};