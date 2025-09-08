#include "GameBoy.h"
#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"
#include "APU.h"
#include "GPU.h"
#include "Input.h"

GameBoy::GameBoy()
	: running(false)
{
	memory = std::make_unique<Memory>();
	cpu = std::make_unique<CPU>(memory.get());
	apu = std::make_unique<APU>();
	gpu = std::make_unique<GPU>();
	input = std::make_unique<Input>();
	cart = std::make_unique<Cartridge>();

	memory->SetCart(cart.get());
	memory->SetGpu(gpu.get());
}

GameBoy::~GameBoy()
{
}

bool GameBoy::Initialise(const std::string& bootRomPath)
{
	if (!memory->LoadBootRom(bootRomPath))
	{
		return false;
	}

	cpu->Reset();

	return true;
}

bool GameBoy::LoadGame(const std::string& cartPath)
{
	if (!cart->LoadCart(cartPath))
	{
		return false;
	}

	running = true;
	return true;
}

void GameBoy::Run()
{
	if (running)
	{
		int cycles = cpu->Step();
		gpu->Step(cycles);

		totalCycles += cycles;
	}
}
