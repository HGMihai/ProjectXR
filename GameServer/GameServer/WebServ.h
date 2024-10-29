#pragma once

class CWebServer
{
public:
	CWebServer();
	void start_server();
	void listen();
	// ----
	int listen_socket;
	bool socket_active;
}; extern CWebServer gWebServer;