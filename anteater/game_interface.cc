#include "anteater_private_pch.hh"

struct s_game_interface_data
{
	s_game_interface_context context;
	c_game_interface_client* client;
	bool applied_game_engine_hooks;
	bool applied_game_engine_host_hooks;
};

static s_game_interface_data g_game_interfaces[k_number_of_titles] = { 0 };

template<e_title k_title_index>
class c_game_engine_host_callback_implementation
{
public:
	static int64_t on_transport_sendto_handler(
		halo_interface::IGameEngineHost* self,
		uint64_t network_id,
		uint8_t* buffer,
		uint32_t length,
		uint32_t port)
	{
		s_game_interface_data* game_interface = &g_game_interfaces[k_title_index];
		int64_t result = game_interface->client->transport_sendto_handler(network_id, buffer, length, port);
		return result;
	}

	static int64_t on_transport_recvfrom_handler(
		halo_interface::IGameEngineHost* self,
		uint8_t* buffer,
		uint32_t length,
		uint64_t* network_id,
		uint32_t port)
	{
		s_game_interface_data* game_interface = &g_game_interfaces[k_title_index];
		int64_t result = game_interface->client->transport_recvfrom_handler(buffer, length, network_id, port);
		return result;
	}
};

static __forceinline transport_sendto_handler_t get_game_engine_host_transport_sendto_handler_callback(
	e_title game_type)
{
	transport_sendto_handler_t result = nullptr;

	switch (game_type)
	{
	case _title_halo1:
		result = c_game_engine_host_callback_implementation<_title_halo1>::on_transport_sendto_handler;
		break;
	case _title_halo2:
		result = c_game_engine_host_callback_implementation<_title_halo2>::on_transport_sendto_handler;
		break;
	case _title_halo3:
		result = c_game_engine_host_callback_implementation<_title_halo3>::on_transport_sendto_handler;
		break;
	case _title_halo4:
		result = c_game_engine_host_callback_implementation<_title_halo4>::on_transport_sendto_handler;
		break;
	case _title_halo2a:
		result = c_game_engine_host_callback_implementation<_title_halo2a>::on_transport_sendto_handler;
		break;
	}

	return result;
}

static __forceinline transport_recvfrom_handler_t get_game_engine_host_transport_recvfrom_handler_callback(
	e_title game_type)
{
	transport_recvfrom_handler_t result = nullptr;

	switch (game_type)
	{
	case _title_halo1:
		result = c_game_engine_host_callback_implementation<_title_halo1>::on_transport_recvfrom_handler;
		break;
	case _title_halo2:
		result = c_game_engine_host_callback_implementation<_title_halo2>::on_transport_recvfrom_handler;
		break;
	case _title_halo3:
		result = c_game_engine_host_callback_implementation<_title_halo3>::on_transport_recvfrom_handler;
		break;
	case _title_halo4:
		result = c_game_engine_host_callback_implementation<_title_halo4>::on_transport_recvfrom_handler;
		break;
	case _title_halo2a:
		result = c_game_engine_host_callback_implementation<_title_halo2a>::on_transport_recvfrom_handler;
		break;
	}

	return result;
}

void anteater_apply_game_engine_host_hooks(
	e_title game_type)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];

	if (game_interface->applied_game_engine_host_hooks)
	{
		return;
	}
	
	c_class_vtable_accessor<k_game_engine_host_vtable_size> game_engine_host_vtable(
		game_interface->context.game_engine_host);

	game_engine_host_vtable.replace_at_index(
		_game_engine_host_vtable_transport_sendto_handler,
		&game_interface->context.transport_sendto_handler,
		get_game_engine_host_transport_sendto_handler_callback(game_type));
	
	game_engine_host_vtable.replace_at_index(
		_game_engine_host_vtable_transport_recvfrom_handler,
		&game_interface->context.transport_recvfrom_handler,
		get_game_engine_host_transport_recvfrom_handler_callback(game_type));

	game_interface->applied_game_engine_host_hooks = true;
}

template<e_title k_title_index>
class c_game_engine_callback_implementation
{
public:
	static void on_PlayGame(
		halo_interface::IGameEngine* self,
		halo_interface::IGameEngineHost* game_engine_host,
		halo_interface::s_game_options* game_options)
	{
		s_game_interface_data* game_interface = &g_game_interfaces[k_title_index];
		
		game_interface->context.game_engine_host = game_engine_host;
		anteater_apply_game_engine_host_hooks(k_title_index);

		game_interface->client->PlayGame(game_options);
	}
};

static __forceinline game_engine_play_game_t get_game_engine_play_game_callback(
	e_title game_type)
{
	game_engine_play_game_t result = nullptr;

	switch (game_type)
	{
	case _title_halo1:
		result = c_game_engine_callback_implementation<_title_halo1>::on_PlayGame;
		break;
	case _title_halo2:
		result = c_game_engine_callback_implementation<_title_halo2>::on_PlayGame;
		break;
	case _title_halo3:
		result = c_game_engine_callback_implementation<_title_halo3>::on_PlayGame;
		break;
	case _title_halo4:
		result = c_game_engine_callback_implementation<_title_halo4>::on_PlayGame;
		break;
	case _title_halo2a:
		result = c_game_engine_callback_implementation<_title_halo2a>::on_PlayGame;
		break;
	}

	return result;
}

void anteater_apply_game_engine_hooks(
	e_title game_type)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];

	if (game_interface->applied_game_engine_hooks)
	{
		return;
	}

	c_class_vtable_accessor<k_game_engine_vtable_size> game_engine_vtable(
		game_interface->context.game_engine);

	game_engine_vtable.replace_at_index(
		_game_engine_vtable_PlayGame,
		&game_interface->context.PlayGame,
		get_game_engine_play_game_callback(game_type));

	game_interface->applied_game_engine_hooks = true;
}

uint32_t game_interface_on_CreateGameEngine(
	e_title game_type,
	halo_interface::IGameEngine** ppEngine)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];
	uint32_t result = game_interface->context.CreateGameEngine(ppEngine);

	game_interface->context.game_engine = *ppEngine;
	anteater_apply_game_engine_hooks(game_type);

	return result;
}

uint32_t game_interface_on_CreateDataAccess(
	e_title game_type,
	halo_interface::IDataAccess** ppDataAccess)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];
	uint32_t result = game_interface->context.CreateDataAccess(ppDataAccess);
	return result;
}

void game_interface_on_SetLibrarySettings(
	e_title game_type,
	halo_interface::s_library_settings* library_settings)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];
	game_interface->context.SetLibrarySettings(library_settings);
}

void anteater_populate_exports(
	HMODULE game_dll_handle,
	s_game_interface_context* game_interface_context)
{
	game_interface_context->game_dll_handle = game_dll_handle;

	game_interface_context->CreateGameEngine = reinterpret_cast<create_game_engine_t>(
		GetProcAddress(game_interface_context->game_dll_handle, "CreateGameEngine"));
	game_interface_context->CreateDataAccess = reinterpret_cast<create_data_access_t>(
		GetProcAddress(game_interface_context->game_dll_handle, "CreateDataAccess"));
	game_interface_context->SetLibrarySettings = reinterpret_cast<set_library_settings_t>(
		GetProcAddress(game_interface_context->game_dll_handle, "SetLibrarySettings"));
}

void game_interface_register_client(
	e_title game_type,
	c_game_interface_client* game_interface_client,
	HMODULE game_dll_handle)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];
	anteater_populate_exports(game_dll_handle, &game_interface->context);
	game_interface->client = game_interface_client;
	game_interface->client->set_context(&game_interface->context);
}

void game_interface_destroy_client(
	e_title game_type)
{
	s_game_interface_data* game_interface = &g_game_interfaces[game_type];
	FreeLibrary(game_interface->context.game_dll_handle);
	delete game_interface->client;
}

c_game_interface_client::c_game_interface_client()
	: m_game_interface_context(nullptr)
{

}

c_game_interface_client::~c_game_interface_client()
{

}

void c_game_interface_client::set_context(
	s_game_interface_context* context)
{
	m_game_interface_context = context;
}

void c_game_interface_client::PlayGame(
	halo_interface::s_game_options* game_options)
{
	m_game_interface_context->PlayGame(
		m_game_interface_context->game_engine,
		m_game_interface_context->game_engine_host,
		game_options);
}

int64_t c_game_interface_client::transport_sendto_handler(
	uint64_t network_id,
	uint8_t* buffer,
	uint32_t length,
	uint32_t port)
{
	int64_t result = m_game_interface_context->transport_sendto_handler(
		m_game_interface_context->game_engine_host,
		network_id,
		buffer,
		length,
		port);
	return result;
}

int64_t c_game_interface_client::transport_recvfrom_handler(
	uint8_t* buffer,
	uint32_t length,
	uint64_t* network_id,
	uint32_t port)
{
	int64_t result = m_game_interface_context->transport_recvfrom_handler(
		m_game_interface_context->game_engine_host,
		buffer,
		length,
		network_id,
		port);
	return result;
}
