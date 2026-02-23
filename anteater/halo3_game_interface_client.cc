#include "anteater_private_pch.hh"

void perform_fault_injection(
	uint8_t *buffer,
	uint32_t size,
	float corruption_fraction)
{
	uint32_t pending_corrupt_byte_count = (uint32_t)((float)size * corruption_fraction);
	while (pending_corrupt_byte_count > 0)
	{
		uint32_t byte_index = rand() % size;
		buffer[byte_index] = rand() % 0x100;

		pending_corrupt_byte_count--;
	}
}

int64_t c_halo3_game_interface_client::transport_sendto_handler(
	uint64_t network_id,
	uint8_t* buffer,
	uint32_t length,
	uint32_t port)
{
	static bool x_mutate = false;
	if (x_mutate)
	{
		perform_fault_injection(buffer, length, .01f);
	}

	int64_t result = c_game_interface_client::transport_sendto_handler(network_id, buffer, length, port);
	if (result > 0)
	{
		DEBUG_LOG("transport_sendto_handler( 0x%llx 0x%p 0x%lx 0x%lx ) = %lld\n", network_id, buffer, length, port, result);
	}

	return result;
}

int64_t c_halo3_game_interface_client::transport_recvfrom_handler(
	uint8_t* buffer,
	uint32_t length,
	uint64_t* network_id,
	uint32_t port)
{
	int64_t result = c_game_interface_client::transport_recvfrom_handler(buffer, length, network_id, port);

	static bool x_mutate = false;
	if (result > 0 && x_mutate)
	{
		DEBUG_LOG("transport_recvfrom_handler( 0x%p 0x%lx 0x%p 0x%lx ) = %lld\n", buffer, length, network_id, port, result);

		perform_fault_injection(buffer, length, .01f);
	}

	return result;
}
