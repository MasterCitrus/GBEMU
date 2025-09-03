#include "GB/GameBoy.h"

int main()
{
	GameBoy gb;

	if (gb.Initialise("bootroms/dmg_boot.bin"))
	{
		if (gb.LoadGame("roms/tetris.gb"))
		{
			gb.Run();
		}
	}
}