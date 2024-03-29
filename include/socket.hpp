#ifndef ISOCKET_H_
#define ISOCKET_H_

#include "console_logger.hpp"

#ifdef __linux__
typedef int SOCKET;
#endif

namespace socktard
{
    class ISocket
    {
    protected:
        SOCKET server_socket;
        struct sockaddr_in server_address;
        unsigned int port;
        unsigned int max_connections;
        std::string ip_address;
        std::string client_ip;
        std::vector<std::string> ip_blacklist;
        std::vector<SOCKET> client_sockets;
        logtard::ConsoleLogger logger;

    public:
        virtual void bindSocket() = 0;
        virtual void listenToConnections() = 0;
        virtual bool acceptConnection(SOCKET &client_socket, void *client_address) = 0;
        virtual long unsigned int receiveData(SOCKET client_socket, std::string *data) = 0;
        virtual void sendData(SOCKET client_socket, const std::string data) = 0;
        virtual void closeSocket() = 0;
        virtual void closeSocket(SOCKET client_socket) = 0;
        virtual void setSocketTimeout(SOCKET sock, int timeoutSec) = 0;

        std::string getClientIp()
        {
            return this->client_ip;
        }
    };
}

#endif
