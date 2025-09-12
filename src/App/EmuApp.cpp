#include "EmuApp.h"
#include "../GB/Cartridge.h"
#include "Core/Window.h"
#include <imgui.h>
#include <nfd.hpp>

bool EmuApp::Startup()
{
    if (!gameboy.Initialise("bootroms/dmg_boot.bin"))
    {
        return false;
    }

    return true;
}

void EmuApp::Shutdown()
{
    
}

void EmuApp::Update(float delta)
{
    stats.fps = fps;
    stats.romCurrentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(stats.romCurrentTime - stats.romStartTime);
    std::chrono::hh_mm_ss<std::chrono::seconds> span(elapsed);
    stats.currentTime = std::format("{:02d}:{:02d}", span.minutes().count(), span.seconds().count());
    gameboy.Run();
}

void EmuApp::Render()
{
    //ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open ROM"))
            {
                NFD::Guard guard;
                NFD::UniquePath path;

                nfdfilteritem_t filter[3] = {
                    {"All ROMs", "gb,gbc"},
                    {"GameBoy", "gb"},
                    {"GameBoy Colour", "gbc"}
                };

                nfdresult_t result = NFD::OpenDialog(path, filter, 3);

                std::string filePath;

                if (result == NFD_OKAY)
                {
                    filePath = path.get();
                    gameboy.LoadGame(filePath);
                    stats.romTitle = gameboy.GetCart()->GetTitle();
                    //GetWindow()->SetWindowTitle(gameboy.GetCart()->GetTitle());
                    gameLoaded = true;
                    stats.romStartTime = std::chrono::steady_clock::now();
                }
                else if (result == NFD_CANCEL)
                {

                }
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("App Stats", &showStats, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text("Current Rom: %s", stats.romTitle.c_str());
        ImGui::Text("FPS: %i", stats.fps);
        if (gameLoaded)
        {
            ImGui::Text("Current Run Time: %s", stats.currentTime.c_str());
        }
    }
    ImGui::End();
}
