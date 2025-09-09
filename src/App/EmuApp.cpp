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
    gameboy.Run();
}

void EmuApp::Render()
{
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
                    GetWindow()->SetWindowTitle(gameboy.GetCart()->GetTitle());
                }
                else if (result == NFD_CANCEL)
                {

                }
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
