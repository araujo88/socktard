#ifdef __linux__
#include "../include/unix_socket.hpp"

using namespace socktard;

UnixSocket::UnixSocket() : UnixSocket(logtard::ConsoleLogger()) {}

UnixSocket::UnixSocket(logtard::ConsoleLogger logger) : UnixSocket(logger, "127.0.0.1", 8888, 128) {}

UnixSocket::UnixSocket(logtard::ConsoleLogger logger, unsigned int port) : UnixSocket(logger, "127.0.0.1", port, 128) {}

UnixSocket::UnixSocket(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port) : UnixSocket(logger, ip_address, port, 128) {}

UnixSocket::UnixSocket(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logger = logger;

    LOG_INFO(logger, "Creating socket ...");
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket < 0)
    {
        throw SocketCreationException();
    }
    LOG_INFO(logger, "Socket created!");

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    // Utils::fillIPBlacklist(this->ip_blacklist);
}

void UnixSocket::bindSocket()
{

    LOG_INFO(logger, "Binding socket ...");
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(this->server_address))) < 0)
    {
        throw SocketBindingException();
    }
    LOG_INFO(logger, "Binding done! Listening to connections ...");
}

void UnixSocket::listenToConnections()
{
    if ((listen(this->server_socket, this->max_connections)) < 0)
    {
        throw SocketListenException();
    }
}

bool UnixSocket::acceptConnection(SOCKET &client_socket, void *client_address)
{
    struct sockaddr_storage client_addr_storage;
    socklen_t client_addr_size = sizeof(client_addr_storage);

    this->setSocketTimeout(this->server_socket, 5);
    client_socket = accept(this->server_socket, (struct sockaddr *)&client_addr_storage, &client_addr_size);
    if (client_socket < 0)
    {
        throw SocketAcceptException();
    }

    if (client_address != nullptr)
    {
        std::memcpy(client_address, &client_addr_storage, client_addr_size);
    }

    char ip_str[INET6_ADDRSTRLEN] = {0};
    if (client_addr_storage.ss_family == AF_INET)
    {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&client_addr_storage;
        inet_ntop(AF_INET, &addr_in->sin_addr, ip_str, INET_ADDRSTRLEN);
    }
    else if (client_addr_storage.ss_family == AF_INET6)
    {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&client_addr_storage;
        inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip_str, INET6_ADDRSTRLEN);
    }

    this->client_ip = std::string(ip_str);

    for (auto it : this->ip_blacklist)
    {
        if (this->client_ip == it)
        {
            return false;
        }
    }

    this->client_sockets.push_back(client_socket);

    return true;
}

long unsigned int UnixSocket::receiveData(SOCKET client_socket, std::string *data)
{
    long unsigned int bytes = recv(client_socket, (void *)(*data).c_str(), (*data).size(), 0);
    if (bytes < 0)
    {
        throw SocketReceiveException();
    }
    return bytes;
}

void UnixSocket::sendData(SOCKET client_socket, const std::string buffer)
{
    if (send(client_socket, buffer.c_str(), buffer.size(), 0) == -1)
    {
        throw SocketSendException();
    }
}

void UnixSocket::closeSocket()
{
    LOG_INFO(logger, "Cleaning up client sockets ...");
    for (auto &it : this->client_sockets)
    {
        this->closeSocket(it);
    }

    LOG_INFO(logger, "Closing socket ...");
    if (shutdown(this->server_socket, SHUT_RDWR) == -1)
    {
        throw SocketCloseException();
    }
    if (close(this->server_socket) == 0)
    {
        LOG_INFO(logger, "Socket closed!");
        return;
    }
    else
    {
        throw SocketCloseException();
    }
}

void UnixSocket::closeSocket(SOCKET client_socket)
{
    if (shutdown(client_socket, SHUT_RDWR) == -1)
    {
        throw SocketCloseException();
    }
    close(client_socket);

    auto it = std::find(this->client_sockets.begin(), this->client_sockets.end(), client_socket);

    // Check if element was found before erasing
    if (it != this->client_sockets.end())
    {
        this->client_sockets.erase(it);
    }
}

void UnixSocket::setSocketTimeout(SOCKET sock, int timeoutSec)
{
    struct timeval tv;
    tv.tv_sec = timeoutSec; // Seconds
    tv.tv_usec = 0;         // Microseconds
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
}

UnixSocket::~UnixSocket() {}

#endif
