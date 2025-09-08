#pragma once
#include "Events/ApplicationEvent.h"

#include <string>

class Window;

class Application
{
public:
	Application();
	~Application();

	bool Initialise(const std::string& title, unsigned int width, unsigned int height, bool fullscreen);
	void Deinitialise();
	// Called in Run()
	// Return true for success
	// Return false for failure
	virtual bool Startup() = 0;
	// Called in Run()
	virtual void Shutdown() = 0;

	void Run();

	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	virtual void OnEvent(Event& e);

	Window* GetWindow() { return window; }

private:
	bool OnWindowClose(WindowCloseEvent& e);

private:
	Window* window;
	bool running;
};