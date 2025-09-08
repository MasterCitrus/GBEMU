#pragma once

#include <memory>
#include <string>

class Cartridge;
class CPU;
class GPU;
class Memory;
class APU;
class Input;

class GameBoy
{
public:
	GameBoy();
	~GameBoy();

	bool Initialise(const std::string& bootRomPath);

	bool LoadGame(const std::string& cartPath);

	void Run();

	Cartridge* GetCart() { return cart.get(); }

private:
	std::unique_ptr<Memory> memory;
	std::unique_ptr<CPU> cpu;
	std::unique_ptr<GPU> gpu;
	std::unique_ptr<APU> apu;
	std::unique_ptr<Input> input;
	std::unique_ptr<Cartridge> cart;
	
	int totalCycles = 0;
	bool running;
};