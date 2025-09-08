#include "App/EmuApp.h"

int main()
{
	EmuApp app;
	if (!app.Initialise("GameBoy Emulator", 800, 600, false))
	{
		return 1;
	}
	app.Run();
}