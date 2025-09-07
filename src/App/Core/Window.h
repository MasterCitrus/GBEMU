#pragma once
#include <string>

class GLFWwindow;

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
	Window(const WindowProps props);
	~Window();

	void Update();

	bool SetVSync(bool enable);
private:
	GLFWwindow* window;
};