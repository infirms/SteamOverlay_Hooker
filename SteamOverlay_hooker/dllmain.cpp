#include "hooker.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        hooks_on_dllmain();

        FlashWindow(GetForegroundWindow(), true);
    }

    return TRUE;
}

