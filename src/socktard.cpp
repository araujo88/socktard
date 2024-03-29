#ifdef __linux__
#include "../include/socktard.hpp"

using namespace socktard;

Socktard::Socktard() : Socktard(logtard::ConsoleLogger()) {}

Socktard::Socktard(logtard::ConsoleLogger logger) : Socktard(logger, "127.0.0.1", 8888, 128) {}

Socktard::Socktard(logtard::ConsoleLogger logger, unsigned int port) : Socktard(logger, "127.0.0.1", port, 128) {}

Socktard::Socktard(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port) : Socktard(logger, ip_address, port, 128) {}

Socktard::Socktard(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections)
{
#ifdef __linux__
    this->socket = socktard::UnixSocket(logger, ip_address, port, max_connections);
#endif
#ifdef _WIN32
    this->socket = socktard::WinSocket(logger, ip_address, port, max_connections);
#endif
}

void Socktard::bindSocket()
{
    this->socket.bindSocket();
}

void Socktard::listenToConnections()
{
    this->socket.listenToConnections();
}

bool Socktard::acceptConnection(SOCKET &client_socket, void *client_address)
{
    return this->socket.acceptConnection(client_socket, client_address);
}

long unsigned int Socktard::receiveData(SOCKET client_socket, std::string *data)
{
    return this->socket.receiveData(client_socket, data);
}

void Socktard::sendData(SOCKET client_socket, const std::string buffer)
{
    return this->socket.sendData(client_socket, buffer);
}

void Socktard::closeSocket()
{
    this->socket.closeSocket();
}

void Socktard::closeSocket(SOCKET client_socket)
{
    this->socket.closeSocket(client_socket);
}

void Socktard::setSocketTimeout(SOCKET sock, int timeoutSec)
{
    this->socket.setSocketTimeout(sock, timeoutSec);
}

Socktard::~Socktard() {}

#endif
