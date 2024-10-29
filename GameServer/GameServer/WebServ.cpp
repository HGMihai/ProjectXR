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
	WSADATA wsaData; // служебная структура для хранение информации
    // о реализации Windows Sockets
    // старт использования библиотеки сокетов процессом
    // (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        LogAdd(LOG_RED, "[WebServer] WSAStartup failed: %d",result);
    	return;
    }

    struct addrinfo* addr = nullptr; // структура, хранящая информацию
    // об IP-адресе  слущающего сокета

    // Шаблон для инициализации структуры адреса
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET определяет, что будет
    // использоваться сеть для работы с сокетом
    hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
    // чтобы принимать входящие соединения

    // Инициализируем структуру, хранящую адрес сокета - addr
    // Наш HTTP-сервер будет висеть на 8000-м порту локалхоста
    result = getaddrinfo("127.0.0.1", "8000", &hints, &addr);

    // Если инициализация структуры адреса завершилась с ошибкой,
    // выведем сообщением об этом и завершим выполнение программы
    if (result != 0) {
        LogAdd(LOG_RED,"[WebServer] getaddrinfo failed: %d", result);
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
    	this->socket_active = false;
        return;
    }

    // Создание сокета
	this->listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    // Если создание сокета завершилось с ошибкой, выводим сообщение,
    // освобождаем память, выделенную под структуру addr,
    // выгружаем dll-библиотеку и закрываем программу
    if (this->listen_socket == INVALID_SOCKET) {
        LogAdd(LOG_RED,"[WebServer] Error at socket: %d", WSAGetLastError());
        freeaddrinfo(addr);
        WSACleanup();
        this->socket_active = false;
        return;
    }

    // Привязываем сокет к IP-адресу
    result = bind(this->listen_socket, addr->ai_addr, static_cast<int>(addr->ai_addrlen));

    // Если привязать адрес к сокету не удалось, то выводим сообщение
    // об ошибке, освобождаем память, выделенную под структуру addr.
    // и закрываем открытый сокет.
    // Выгружаем DLL-библиотеку из памяти и закрываем программу.
    if (result == SOCKET_ERROR) {
        LogAdd(LOG_RED,"[WebServer] bind failed with error: %d", WSAGetLastError());
        freeaddrinfo(addr);
        closesocket(this->listen_socket);
        WSACleanup();
        this->socket_active = false;
        return;
    }

    // Инициализируем слушающий сокет
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
	// Принимаем входящие соединения
        client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            LogAdd(LOG_RED,"[WebServer] accept failed: %d",WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        int result = recv(client_socket, buf, max_client_buffer_size, 0);

        std::stringstream response; // сюда будет записываться ответ клиенту
        std::stringstream response_body; // тело ответа

        if (result == SOCKET_ERROR) {
            // ошибка получения данных
            LogAdd(LOG_RED,"[WebServer] recv failed: %d", result);
            closesocket(client_socket);
        } else if (result == 0) {
            // соединение закрыто клиентом
            LogAdd(LOG_RED,"[WebServer] connection closed...");
        } else if (result > 0) {
            // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
            // В буфере запроса.
            buf[result] = '\0';

        	
            // Данные успешно получены
            // формируем тело ответа (HTML)
            //response_body << "<title>Test C++ HTTP Server</title>\n"
            //    << "<h1>Test page</h1>\n"
            //    << "<p>This is body of the test page...</p>\n"
            //    << "<h2>Request headers</h2>\n"
            //    << "<pre>" << buf << "</pre>\n"
            //    << "<em><small>Test C++ Http Server</small></em>\n";

            //// Формируем весь ответ вместе с заголовками
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

            // Формируем весь ответ вместе с заголовками
            response << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << response_body.str().length()
                << "\r\n\r\n"
                << response_body.str();

            // Отправляем ответ клиенту с помощью функции send
            result = send(client_socket, response.str().c_str(),
                response.str().length(), 0);

            if (result == SOCKET_ERROR) {
                // произошла ошибка при отправле данных
                LogAdd(LOG_RED,"[WebServer] send failed: %d", WSAGetLastError());
            }
            // Закрываем соединение к клиентом
            closesocket(client_socket);
        }
}
