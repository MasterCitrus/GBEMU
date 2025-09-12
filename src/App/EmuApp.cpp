#include "EmuApp.h"
#include "../GB/Cartridge.h"
#include "../GB/Stats.h"
#include "../GB/Register.h"
#include "Core/Window.h"
#include <imgui.h>
#include <nfd.hpp>
#include <format>

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
        CPUStats cpuStats = gameboy.GetCPUStats();
        std::string PC = std::format("{:#04X}", *(cpuStats.PC));
        std::string AF = std::format("{:#04X}", cpuStats.registers->AF);
        std::string A = std::format("{:#02X}", cpuStats.registers->A);
        std::string F = std::format("{:#02X}", cpuStats.registers->F);
        std::string BC = std::format("{:#04X}", cpuStats.registers->BC);
        std::string B = std::format("{:#02X}", cpuStats.registers->B);
        std::string C = std::format("{:#02X}", cpuStats.registers->C);
        std::string DE = std::format("{:#04X}", cpuStats.registers->DE);
        std::string D = std::format("{:#02X}", cpuStats.registers->D);
        std::string E = std::format("{:#02X}", cpuStats.registers->E);
        std::string HL = std::format("{:#04X}", cpuStats.registers->HL);
        std::string H = std::format("{:#02X}", cpuStats.registers->H);
        std::string L = std::format("{:#02X}", cpuStats.registers->L);
        std::string OP = std::format("{:#02X}", *(cpuStats.OP));
        ImGui::SeparatorText("CPU Stats");
        ImGui::Text("PC: %s", PC.c_str());
        ImGui::Text("OP: %s", OP.c_str());
        ImGui::Text("AF: %s", AF.c_str());
        ImGui::Text("A: %s", A.c_str());
        ImGui::Text("F: %s", F.c_str());
        ImGui::Text("BC: %s", BC.c_str());
        ImGui::Text("B: %s", B.c_str());
        ImGui::Text("C: %s", C.c_str());
        ImGui::Text("DE: %s", DE.c_str());
        ImGui::Text("D: %s", D.c_str());
        ImGui::Text("E: %s", E.c_str());
        ImGui::Text("HL: %s", HL.c_str());
        ImGui::Text("H: %s", H.c_str());
        ImGui::Text("L: %s", L.c_str());
        ImGui::Text("Zero Flag: %s", cpuStats.registers->GetZeroFlag() ? "True" : "False");
        ImGui::Text("Subtract Flag: %s", cpuStats.registers->GetSubtractFlag() ? "True" : "False");
        ImGui::Text("Half Carry Flag: %s", cpuStats.registers->GetHalfCarryFlag() ? "True" : "False");
        ImGui::Text("Carry Flag: %s", cpuStats.registers->GetCarryFlag() ? "True" : "False");
    }
    ImGui::End();
}
