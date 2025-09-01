#pragma once

#include "Types.h"

enum class MemoryArea
{
	ROM_BANK_0,
	ROM_BANK_N,
	VIDEO_RAM,
	EXTERNAL_RAM,
	WORK_RAM_BANK_0,
	WORK_RAM_BANK_1,
	ECHO_RAM,
	OAM,
	PROHIBITIED,
	IO_REGISTERS,
	HIGH_RAM,
	INTERRUPT_ENABLE_REGISTER
};

class MMU
{
public:
	

private:
	constexpr MemoryArea GetMemoryArea(const u16 address) const;

private:

	u8 memory[0xFFFF] = { 0 };
};