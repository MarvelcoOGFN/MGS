#include "minhook/MinHook.h"
#include "Utils.h"
#include "Hooking.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace std;
using namespace SDK;

DWORD InitThread(LPVOID)
{
    AllocConsole();
    FILE* fptr;
    freopen_s(&fptr, "CONOUT$", "w+", stdout);

    MH_Initialize();
    Log("Initializing MGS");

    Gamemode::InitHooks();
    Tick::InitHooks();
    PC::InitHooks();
    Misc::InitHooks();

    MH_EnableHook(MH_ALL_HOOKS);

    *(bool*)(__int64(GetModuleHandleW(0)) + OFFSET) = false; //GIsClient
    *(bool*)(__int64(GetModuleHandleW(0)) + OFFSET) = true; //GIsServer

    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"open Apollo_Terrain", nullptr);
    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0); 
   
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID lpReserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, InitThread, 0, 0, 0);
        break;
    default:
        break;
    }
    return TRUE;
}