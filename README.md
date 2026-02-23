# MCC packet interceptor

This project installs a hook into the sendto/recvfrom interfaces the MCC game
host (UE layer) exposes to game DLL clients through `halo_interface::IGameEngineHost`.
The game uses packet encryption but we can access the plain data with this.

Currently missing from repo: packet dumper and some local documentation (out
of band message format, simulation updates, etc.) The intent is to generate
valid permutations of message bitstreams from scratch and create mock sessions.
