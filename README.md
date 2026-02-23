# MCC packet interceptor

This project installs a hook into the sendto/recvfrom interfaces the MCC game
host (UE layer) exposes to game DLL clients through `halo_interface::IGameEngineHost`.
The game uses packet encryption but we can access the plain data with this.

Currently missing from repo: packet dumper and some local documentation (out
of band message format, simulation updates, etc.) The intent is to generate
valid permutations of message bitstreams from scratch and create mock sessions.

## Networking brief

When recieving a packet c_network_channel will iterate over each of its
registered c_network_channel_clients through ``e_network_read_result c_network_channel_client::read_from_packet(uint32 *packet_sequence_number, c_bitstream *packet)``
until one returns a non-ok result:

```
network_recieve
recvfrom...
	-> c_network_link::process_incoming_packets
		-> c_network_link::process_packet_internal: c_network_link::get_associated_channel(&packet->address)
			-> c_network_channel::receive_packet: iterator.client->read_from_packet
```

```cpp
enum e_network_read_result
{
    _network_read_result_ok,
    _network_read_result_premature_stop,
    _network_read_result_discard,
    _network_read_result_corrupt,
    k_network_read_result_count
};
```

### Replication

Replication is the layer in Halo 3's networking stack which sends and recieves
simulation data from peers ingesting it into the local game simulation.
Simulation updates come in entities and events. Entities are persistent objects
with updatable fields and a baseline. Events are things that happen once and
operate on extant entities: weapons firing, melee attacks, kick requests, etc.

```
c_replication_scheduler::read_from_packet: v13->read_from_packet ->
	c_replication_event_manager_view::read_from_packet
		-> c_simulation_event_handler::read_incoming_event
			-> event_definition->event_payload_decode
	c_replication_entity_manager_view::read_from_packet
		-> c_simulation_entity_database::read_creation_from_packet
		-> c_simulation_entity_database::read_update_from_packet
	c_replication_control_view::read_from_packet
```
