#pragma once

class c_halo3_game_interface_client
	: public c_game_interface_client
{
public:
	int64_t transport_sendto_handler(uint64_t network_id, uint8_t* buffer, uint32_t length, uint32_t port) override;
	int64_t transport_recvfrom_handler(uint8_t* buffer, uint32_t length, uint64_t* network_id, uint32_t port) override;
};
