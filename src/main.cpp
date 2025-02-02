#define WIN32_LEAN_AND_MEAN
#include "mem/snakemem.h"
#include "offsets.h"
#include <iostream>
#include <thread>

int main() {
    // process Init
    SnakeMem mem("cs2");

    // getting processId
    DWORD procId = mem.GetProcessId();
    if (!procId) {
        std::cout << "Searching process \"cs2.exe\"..." << std::endl;
        do {
            procId = mem.GetProcessId();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (!procId);
    }
    std::cout << "Process Found! Process ID: " << procId << std::endl;
    
    // Getting client.dll address
    uintptr_t client = mem.GetModuleAddress("client.dll");
    if (!client) {
        std::cout << "Error: \"client.dll\" not found" << std::endl;
        std::cout << "Press ENTER to exit ";
        system("pause > nul");
        return 1;
    }
    
    while (true) {
        // get entity list and localplayer
        uintptr_t entityList = mem.ReadPointer(client, offsets::client_dll::dwEntityList);
        uintptr_t localPlayerPawn = mem.ReadPointer(client, offsets::client_dll::dwLocalPlayerPawn);

        uint16_t team = mem.ReadPointer(localPlayerPawn, {});

        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    return 0;
}