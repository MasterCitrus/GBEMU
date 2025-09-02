#include "Register.h"

Registers::Registers()
	: AF(0x0), BC(0x0), DE(0x0), HL(0x0)
{
}

bool Registers::GetZeroFlag()
{
	return (F & ZERO) != 0;
}

bool Registers::GetSubtractFlag()
{
	return (F & SUBTRACT) != 0;
}

bool Registers::GetHalfCarryFlag()
{
	return (F & HALFCARRY) != 0;
}

bool Registers::GetCarryFlag()
{
	return (F & CARRY) != 0;
}

void Registers::SetZeroFlag(bool value)
{
	F = (F & 0x7F) | (value ? ZERO : 0);
}

void Registers::SetSubtractFlag(bool value)
{
	F = (F & 0xBF) | (value ? SUBTRACT : 0);
}

void Registers::SetHalfCarryFlag(bool value)
{
	F = (F & 0xDF) | (value ? HALFCARRY : 0);
}

void Registers::SetCarryFlag(bool value)
{
	F = (F & 0xEF) | (value ? CARRY : 0);
}
