#define WIN32_LEAN_AND_MEAN
#include "mem/snakemem.h"
#include "offsets.h"
#include <iostream>
#include <thread>

#define HOTKEY 0x24 // Triggerbot hotkey

int main() {
    SetConsoleTitle("INeed");

    // process Init
    SnakeMem mem("cs2");

    // getting processId
    DWORD procId = mem.GetProcessId();
    if (!procId) {
        std::cout << "[~] Searching for process \"cs2.exe\"..." << std::endl;
        uint8_t breakFlag{ 1 };
        do {
            if (breakFlag >= 100) { // If 10 seconds have elapsed (100 * 100 ms)
                std::cout << "[!] CS2 is not running! The program is shutting down." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return 0;
            }

            if (breakFlag % 20 == 0) { // Every 2 seconds (20 * 100 ms)
                std::cout << "[~] Waiting for CS2 to start..." << std::endl;
            }

            procId = mem.GetProcessId();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            breakFlag += 1;
        } while (!procId);
    }
    std::cout << "[+] Process Found! Process ID: " << procId << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Getting client.dll address
    uintptr_t client = mem.GetModuleAddress("client.dll");
    if (!client) {
        std::cout << "[!] Error: \"client.dll\" not found" << std::endl;
        std::cout << "[~] Press ENTER to exit ";
        system("pause > nul");
        return 1;
    }
    std::cout << "client.dll -> 0x" << std::hex << client << std::dec << std::endl;

    std::cout << std::endl;

    bool triggerbotEnabled = false; // triggerbot Flag
    std::cout << "[+] TriggerBot started! Press HOME for Enable/Disable TriggerBot." << std::endl;
    
    while (true) {
        if (!mem.GetProcessId()) {
            std::cout << "[!] Game closed or not found!" << std::endl;
            std::cout << "Press ENTER to exit" << std::endl;
            system("pause > nul");
        }
        if (GetAsyncKeyState(HOTKEY) & 1) {
            triggerbotEnabled = !triggerbotEnabled;
            std::cout << "[+] TriggerBot " << (triggerbotEnabled ? "enabled!" : "disabled!") << std::endl;
        }

        if (!triggerbotEnabled) continue;

        // get entity list and localplayer
        uintptr_t entityList = mem.ReadPointer(client, offsets::client_dll::dwEntityList);
        uintptr_t localPlayerPawn = mem.ReadPointer(client, offsets::client_dll::dwLocalPlayerPawn);

        // getting local team and crosshair id
        uint8_t localTeam = mem.ReadPointer(localPlayerPawn, offsets::client_dll::m_iTeamNum);
        int entIndex = mem.ReadPointer(localPlayerPawn, offsets::client_dll::m_iIDEntIndex);

        std::cout << "[+] Crosshair/Entity ID: " << entIndex << std::endl;

        if (entIndex != -1) {
            // Getting entity controller from index
            uintptr_t listEntry = mem.ReadPointer(entityList, 0x8 * ((entIndex & 0x7FFF) >> 9) + 0x10);

            // Getting entity Pawn
            uintptr_t entPawn = mem.ReadPointer(listEntry, 0x78 * (entIndex & 0x1FF));

            // Getting Entity Team Number
            uint8_t entityTeam = mem.ReadPointer(entPawn, offsets::client_dll::m_iTeamNum);
            if (localTeam != entityTeam) { // check if entity is enemy
                mem.Write(client + offsets::buttons::attack, 65537);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                mem.Write(client + offsets::buttons::attack, 256);
            }
            
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}