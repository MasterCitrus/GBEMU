#pragma once
#include "Core/Application.h"
#include "../GB/GameBoy.h"
#include <chrono>

class GameBoy;

struct AppStats
{
	std::string romTitle = "None";
	unsigned int fps = 0;
	std::chrono::steady_clock::time_point romStartTime;
	std::chrono::steady_clock::time_point romCurrentTime;
	std::string currentTime;
};

class EmuApp : public Application
{
public:


	virtual bool Startup() override;
	virtual void Shutdown() override;

	virtual void Update(float delta) override;
	virtual void Render() override;

private:
	GameBoy gameboy;
	AppStats stats;
	bool showStats = true;
	bool gameLoaded = false;

};