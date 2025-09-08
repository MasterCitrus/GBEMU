#pragma once
#include "Events/Event.h"
#include <functional>
#include <string>

struct GLFWwindow;

struct WindowProps
{
	std::string title;
	unsigned int width;
	unsigned int height;
	bool fullscreen;
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& props);
	~Window();

	void Init();
	void Shutdown();

	void Update();

	void SetEventCallback(const EventCallbackFn& callback) { data.callback = callback; }
	void SetVSync(bool enable);
	void SetWindowTitle(const std::string& title);

	GLFWwindow* GetWindow() { return window; }
private:
	GLFWwindow* window;

	struct WindowData
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool fullscreen;
		bool vsync;
		EventCallbackFn callback;
	};

	WindowData data;
};