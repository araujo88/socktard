#ifdef __linux__
#ifndef UNIX_SOCKET_H_
#define UNIX_SOCKET_H_

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include "socket.hpp"
#include "exceptions.hpp"
#include "console_logger.hpp"

namespace socktard
{
    class UnixSocket : public ISocket
    {
    public:
        UnixSocket();
        UnixSocket(logtard::ConsoleLogger logger);
        UnixSocket(logtard::ConsoleLogger logger, unsigned int port);
        UnixSocket(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port);
        UnixSocket(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections);
        ~UnixSocket();
        virtual void bindSocket() override;
        virtual void listenToConnections() override;
        virtual bool acceptConnection(SOCKET &client_socket, void *client_address) override;
        virtual long unsigned int receiveData(SOCKET client_socket, std::string *data) override;
        virtual void sendData(SOCKET client_socket, const std::string bytes) override;
        virtual void closeSocket() override;
        virtual void closeSocket(SOCKET client_socket) override;
        virtual void setSocketTimeout(SOCKET sock, int timeoutSec) override;
    };
}

#endif
#endif
