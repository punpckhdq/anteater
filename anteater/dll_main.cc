#include "anteater_private_pch.hh"

extern "C"
{
	__declspec(dllexport) uint32_t CreateGameEngine(halo_interface::IGameEngine** ppEngine);
	__declspec(dllexport) uint32_t CreateDataAccess(halo_interface::IDataAccess** ppDataAccess);
	__declspec(dllexport) void SetLibrarySettings(halo_interface::s_library_settings* library_settings);
}

uint32_t CreateGameEngine(
	halo_interface::IGameEngine** ppEngine)
{
	uint32_t result = game_interface_on_CreateGameEngine(_title_halo3, ppEngine);
	return result;
}

uint32_t CreateDataAccess(
	halo_interface::IDataAccess** ppDataAccess)
{
	uint32_t result = game_interface_on_CreateDataAccess(_title_halo3, ppDataAccess);
	return result;
}

void SetLibrarySettings(
	halo_interface::s_library_settings* library_settings)
{
	game_interface_on_SetLibrarySettings(_title_halo3, library_settings);
}

void anteater_initialize(
	void)
{
#if 1
	while (!IsDebuggerPresent())
	{
		Sleep(100);
	}
#endif

	HMODULE halo3_dll_handle = LoadLibraryA("F:\\SteamLibrary\\steamapps\\common\\Halo The Master Chief Collection\\halo3\\halo3_dllcache_play_UUI_x64.dll");
	c_halo3_game_interface_client* halo3_game_interface_client = new c_halo3_game_interface_client();
	game_interface_register_client(_title_halo3, halo3_game_interface_client, halo3_dll_handle);
}

void anteater_dispose(
	void)
{
	game_interface_destroy_client(_title_halo3);
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		anteater_initialize();
		break;
	case DLL_PROCESS_DETACH:
		anteater_dispose();
		break;
	}

	return TRUE;
}
