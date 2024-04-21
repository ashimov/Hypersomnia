#pragma once

struct masterserver_entry_meta {
	// GEN INTROSPECTOR struct masterserver_entry_meta
	double time_hosted = 0.0;
	bool is_official = false;
	// END GEN INTROSPECTOR
};

struct masterserver_client {
	double time_last_heartbeat = -1;

	masterserver_entry_meta meta;
	server_heartbeat last_heartbeat;
};

using server_list_type = std::unordered_map<webrtc_id_type, masterserver_client>;

