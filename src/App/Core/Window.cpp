#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

Window::Window(const WindowProps& props)
{
	data.title = props.title;
	data.height = props.height;
	data.width = props.width;
	data.fullscreen = props.fullscreen;

	Init();
}

Window::~Window()
{
	Shutdown();
}

void Window::Init()
{
	int glfw = glfwInit();
	if (!glfw)
	{
		__debugbreak();
	}

	window = glfwCreateWindow(data.width, data.height, data.title.c_str(), data.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		__debugbreak();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		__debugbreak();
	}

	glfwSetWindowUserPointer(window, &data);
	SetVSync(true);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
							  {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								  data.width = width;
								  data.height = height;

								  WindowResizeEvent event(width, height);
								  data.callback(event);
							  });

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
							   {
								   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								   WindowCloseEvent event;
								   data.callback(event);
							   });

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
					   {
						   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

						   switch (action)
						   {
							   case GLFW_PRESS:
							   {
								   KeyPressedEvent event(key, false);
								   data.callback(event);
								   break;
							   }
							   case GLFW_RELEASE:
							   {
								   KeyReleasedEvent event(key);
								   data.callback(event);
								   break;
							   }
							   case GLFW_REPEAT:
							   {
								   KeyPressedEvent event(key, true);
								   data.callback(event);
								   break;
							   }
						   }
					   });

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
						{
							WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							KeyTypedEvent event(keycode);
							data.callback(event);
						});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
							   {
								   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								   switch (action)
								   {
									   case GLFW_PRESS:
									   {
										   MouseButtonPressedEvent event(button);
										   data.callback(event);
										   break;
									   }
									   case GLFW_RELEASE:
									   {
										   MouseButtonReleasedEvent event(button);
										   data.callback(event);
										   break;
									   }
								   }
							   });

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
						  {
							  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							  MouseScrolledEvent event((float)xOffset, (float)yOffset);
							  data.callback(event);
						  });

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
							 {
								 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								 MouseMovedEvent event((float)xPos, (float)yPos);
								 data.callback(event);
							 });

	glfwSetWindowRefreshCallback(window, [](GLFWwindow* window)
								 {
									 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									 WindowRefreshEvent event;
									 data.callback(event);
								 });
}

void Window::SetVSync(bool enable)
{
	if (enable) glfwSwapInterval(1);
	else glfwSwapInterval(0);

	data.vsync = enable;
}

void Window::SetWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(window, (data.title + " | " + title).c_str());
}

void Window::Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::Update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}