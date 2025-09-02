#include "GB/GameBoy.h"

int main()
{
	GameBoy gb;

	gb.Initialise("bootroms/dmg_boot.bin");

	gb.LoadGame("roms/tetris.gb");

	gb.Run();
}