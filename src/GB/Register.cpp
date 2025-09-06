#include "Register.h"

Registers::Registers()
	: AF(0x0), BC(0x0), DE(0x0), HL(0x0)
{
}

bool Registers::GetZeroFlag()
{
	return (F >> 7) & (u8)1;
}

bool Registers::GetSubtractFlag()
{
	return (F >> 6) & (u8)1;
}

bool Registers::GetHalfCarryFlag()
{
	return (F >> 5) & (u8)1;
}

bool Registers::GetCarryFlag()
{
	return (F >> 4) & (u8)1;
}

void Registers::SetZeroFlag(bool value)
{
	
	F = (F & ~((u8)1 << 7)) | ((u8)value << 7);

}

void Registers::SetSubtractFlag(bool value)
{
	F = (F & ~((u8)1 << 6)) | ((u8)value << 6);
}

void Registers::SetHalfCarryFlag(bool value)
{
	F = (F & ~((u8)1 << 5)) | ((u8)value << 5);
}

void Registers::SetCarryFlag(bool value)
{
	F = (F & ~((u8)1 << 4)) | ((u8)value << 4);
}
