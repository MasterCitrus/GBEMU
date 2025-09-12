#include "App/EmuApp.h"
#include "GB/GameBoy.h"

int main()
{
	GameBoy gameboy;
	gameboy.Initialise("bootroms/dmg_boot.bin");
	gameboy.LoadGame("roms/cpu_instrs/cpu_instrs.gb");

	while (1)
	{
		gameboy.Run();
	}
	//EmuApp app;
	//if (!app.Initialise("GameBoy Emulator", 800, 600, false))
	//{
	//	return 1;
	//}
	//app.Run();
}