#pragma once

#include "Types.h"

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

	// Stack Pointer
	u16 SP;
	// Program Counter
	u16 PC;
};