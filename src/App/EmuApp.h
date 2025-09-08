#pragma once
#include "Core/Application.h"

class GameBoy;

class EmuApp : public Application
{
public:


	virtual bool Startup() override;
	virtual void Shutdown() override;

	virtual void Update(float delta) override;
	virtual void Render() override;

private:
	GameBoy* gameboy;
};