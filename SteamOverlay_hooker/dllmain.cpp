#include "hooker.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
   
    if (!GetModuleHandleA("GameOverlayRenderer.dll"))
    {
        MessageBoxA(NULL, "Are you sure that GameOverlayRenderer.dll is loaded?", "Module not found!", MB_ICONERROR);
        FreeLibrary(GetModuleHandle(0));
    }
    else
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::cout << "Attached to dll!\n" << endl;
        hooks_on_dllmain();
        FlashWindow(GetForegroundWindow(), true);
    }
    return TRUE;
}

