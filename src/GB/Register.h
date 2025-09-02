#pragma once

#include "Types.h"

enum Flags : u8
{
	ZERO = 1 << 7,
	SUBTRACT = 1 << 6,
	HALFCARRY = 1 << 5,
	CARRY = 1 << 4
};

struct Registers
{
	Registers();

	// AF Register
	struct
	{
		union
		{
			struct
			{
				u8 F;
				u8 A;
			};

			u16 AF;
		};
	};

	// BC Register
	struct
	{
		union
		{
			struct
			{
				u8 C;
				u8 B;
			};

			u16 BC;
		};
	};

	// DE Register
	struct
	{
		union
		{
			struct
			{
				u8 E;
				u8 D;
			};

			u16 DE;
		};
	};

	// HL Register
	struct
	{
		union
		{
			struct
			{
				u8 L;
				u8 H;
			};

			u16 HL;
		};
	};

	bool GetZeroFlag();
	bool GetSubtractFlag();
	bool GetHalfCarryFlag();
	bool GetCarryFlag();

	void SetZeroFlag(bool value);
	void SetSubtractFlag(bool value);
	void SetHalfCarryFlag(bool value);
	void SetCarryFlag(bool value);
};