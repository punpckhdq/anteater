#pragma once

struct s_game_interface_context
{
	HMODULE game_dll_handle;
	create_game_engine_t CreateGameEngine;
	create_data_access_t CreateDataAccess;
	set_library_settings_t SetLibrarySettings;

	halo_interface::IGameEngine* game_engine;
	game_engine_play_game_t PlayGame;

	halo_interface::IGameEngineHost* game_engine_host;
	transport_sendto_handler_t transport_sendto_handler;
	transport_recvfrom_handler_t transport_recvfrom_handler;
};

class c_game_interface_client
{
public:
	c_game_interface_client();
	virtual ~c_game_interface_client();

	virtual void set_context(s_game_interface_context* context);
	virtual void PlayGame(halo_interface::s_game_options* game_options);
	virtual int64_t transport_sendto_handler(uint64_t network_id, uint8_t* buffer, uint32_t length, uint32_t port);
	virtual int64_t transport_recvfrom_handler(uint8_t* buffer, uint32_t length, uint64_t* network_id, uint32_t port);
protected:
	s_game_interface_context* m_game_interface_context;
};

enum e_title
{
	_title_halo1,
	_title_halo2,
	_title_halo3,
	_title_halo4,
	_title_halo2a,
	k_number_of_titles
};

uint32_t game_interface_on_CreateGameEngine(e_title game_type, halo_interface::IGameEngine** ppEngine);
uint32_t game_interface_on_CreateDataAccess(e_title game_type, halo_interface::IDataAccess** ppDataAccess);
void  game_interface_on_SetLibrarySettings(e_title game_type, halo_interface::s_library_settings* library_settings);
void game_interface_register_client(e_title game_type, c_game_interface_client* game_interface_client, HMODULE game_dll_handle);
void game_interface_destroy_client(e_title game_type);
