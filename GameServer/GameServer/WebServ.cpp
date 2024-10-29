#include "stdafx.h"
#include "WebServ.h"
#include "Util.h"
#include <WS2tcpip.h>
CWebServer gWebServer;

CWebServer::CWebServer()
{
	this->socket_active = true;
	this->listen_socket = 0;
	this->start_server();
}

void CWebServer::start_server()
{
	WSADATA wsaData; // ��������� ��������� ��� �������� ����������
    // � ���������� Windows Sockets
    // ����� ������������� ���������� ������� ���������
    // (������������ Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // ���� ��������� ������ ��������� ����������
    if (result != 0) {
        LogAdd(LOG_RED, "[WebServer] WSAStartup failed: %d",result);
    	return;
    }

    struct addrinfo* addr = nullptr; // ���������, �������� ����������
    // �� IP-������  ���������� ������

    // ������ ��� ������������� ��������� ������
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET ����������, ��� �����
    // �������������� ���� ��� ������ � �������
    hints.ai_socktype = SOCK_STREAM; // ������ ��������� ��� ������
    hints.ai_protocol = IPPROTO_TCP; // ���������� �������� TCP
    hints.ai_flags = AI_PASSIVE; // ����� ����� ��������� �� �����,
    // ����� ��������� �������� ����������

    // �������������� ���������, �������� ����� ������ - addr
    // ��� HTTP-������ ����� ������ �� 8000-� ����� ����������
    result = getaddrinfo("127.0.0.1", "8000", &hints, &addr);

    // ���� ������������� ��������� ������ ����������� � �������,
    // ������� ���������� �� ���� � �������� ���������� ���������
    if (result != 0) {
        LogAdd(LOG_RED,"[WebServer] getaddrinfo failed: %d", result);
        WSACleanup(); // �������� ���������� Ws2_32.dll
    	this->socket_active = false;
        return;
    }

    // �������� ������
	this->listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    // ���� �������� ������ ����������� � �������, ������� ���������,
    // ����������� ������, ���������� ��� ��������� addr,
    // ��������� dll-���������� � ��������� ���������
    if (this->listen_socket == INVALID_SOCKET) {
        LogAdd(LOG_RED,"[WebServer] Error at socket: %d", WSAGetLastError());
        freeaddrinfo(addr);
        WSACleanup();
        this->socket_active = false;
        return;
    }

    // ����������� ����� � IP-������
    result = bind(this->listen_socket, addr->ai_addr, static_cast<int>(addr->ai_addrlen));

    // ���� ��������� ����� � ������ �� �������, �� ������� ���������
    // �� ������, ����������� ������, ���������� ��� ��������� addr.
    // � ��������� �������� �����.
    // ��������� DLL-���������� �� ������ � ��������� ���������.
    if (result == SOCKET_ERROR) {
        LogAdd(LOG_RED,"[WebServer] bind failed with error: %d", WSAGetLastError());
        freeaddrinfo(addr);
        closesocket(this->listen_socket);
        WSACleanup();
        this->socket_active = false;
        return;
    }

    // �������������� ��������� �����
    if (::listen(this->listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        LogAdd(LOG_RED,"[WebServer] listen failed with error: %d", WSAGetLastError());
        closesocket(this->listen_socket);
        WSACleanup();
        this->socket_active = false;
        return;
    }
}

#include <Http.h>

const int max_client_buffer_size = 1024;
char buf[max_client_buffer_size];
int client_socket = INVALID_SOCKET;
#include <sstream>
void CWebServer::listen()
{
	if (!this->socket_active)
	{
		return;
	}
	// ��������� �������� ����������
        client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            LogAdd(LOG_RED,"[WebServer] accept failed: %d",WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        int result = recv(client_socket, buf, max_client_buffer_size, 0);

        std::stringstream response; // ���� ����� ������������ ����� �������
        std::stringstream response_body; // ���� ������

        if (result == SOCKET_ERROR) {
            // ������ ��������� ������
            LogAdd(LOG_RED,"[WebServer] recv failed: %d", result);
            closesocket(client_socket);
        } else if (result == 0) {
            // ���������� ������� ��������
            LogAdd(LOG_RED,"[WebServer] connection closed...");
        } else if (result > 0) {
            // �� ����� ����������� ������ ���������� ������, ������� ������ ����� ����� ������
            // � ������ �������.
            buf[result] = '\0';

        	
            // ������ ������� ��������
            // ��������� ���� ������ (HTML)
            //response_body << "<title>Test C++ HTTP Server</title>\n"
            //    << "<h1>Test page</h1>\n"
            //    << "<p>This is body of the test page...</p>\n"
            //    << "<h2>Request headers</h2>\n"
            //    << "<pre>" << buf << "</pre>\n"
            //    << "<em><small>Test C++ Http Server</small></em>\n";

            //// ��������� ���� ����� ������ � �����������
            //response << "HTTP/1.1 200 OK\r\n"
            //    << "Version: HTTP/1.1\r\n"
            //    << "Content-Type: text/html; charset=utf-8\r\n"
            //    << "Content-Length: " << response_body.str().length()
            //    << "\r\n\r\n"
            //    << response_body.str();

        	 response_body << "<pre>" << buf << "</pre>";

        	std::string text = buf;

        	/*if (text.find("100-Continue") != std::string::npos ||
				text.find("100-continue") != std::string::npos)
        	{
        		LogAdd(LOG_TEST,"find 100 continue");
        		return;
        	}*/

        	//100-continue

        	//MsgBox(buf);
        	LogAdd(LOG_RED,"%s",buf);

            // ��������� ���� ����� ������ � �����������
            response << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << response_body.str().length()
                << "\r\n\r\n"
                << response_body.str();

            // ���������� ����� ������� � ������� ������� send
            result = send(client_socket, response.str().c_str(),
                response.str().length(), 0);

            if (result == SOCKET_ERROR) {
                // ��������� ������ ��� �������� ������
                LogAdd(LOG_RED,"[WebServer] send failed: %d", WSAGetLastError());
            }
            // ��������� ���������� � ��������
            closesocket(client_socket);
        }
}
