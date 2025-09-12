#include "Application.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <chrono>

Application::Application()
{
}

Application::~Application()
{
	
}

bool Application::Initialise(const std::string& title, unsigned int width, unsigned int height, bool fullscreen)
{
	window = new Window({title, width, height, fullscreen});
	if (!window)
	{
		return false;
	}
	window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	GLFWwindow* nativeWin = window->GetWindow();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(nativeWin, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return true;
}

void Application::Deinitialise()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	window->Shutdown();
}

void Application::Run()
{
	if (!Startup())
	{
		Deinitialise();
		return;
	}

	running = true;

	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::high_resolution_clock> currTime;

	double deltaTime = 0;
	unsigned int frames = 0;
	double fpsInterval = 0;

	while (running)
	{
		
		currTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime);
		prevTime = currTime;

		deltaTime = duration.count() / 1000.0f;

		frames++;
		fpsInterval += deltaTime;
		if (fpsInterval >= 1.0f)
		{
			fps = frames;
			frames = 0;
			fpsInterval = 0;
		}

		//if (glfwGetWindowAttrib(window->GetWindow(), GLFW_ICONIFIED) != 0) continue;

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Update((float)deltaTime);
		Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window->Update();
	}

	Shutdown();
	Deinitialise();
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	running = false;
	return true;
}
