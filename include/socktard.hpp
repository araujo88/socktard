#ifndef SOCKTARD_H_
#define SOCKTARD_H_

#ifdef __linux__
#include "../include/unix_socket.hpp"
#endif
#ifdef _WIN32
#include "../win_socket.hpp"
#endif

namespace socktard
{
    class Socktard : public ISocket
    {
    private:
#ifdef __linux__
        socktard::UnixSocket socket;
#endif
#ifdef _WIN32
        socktard::WinSocket socket;
#endif

    public:
        Socktard();
        Socktard(logtard::ConsoleLogger logger);
        Socktard(logtard::ConsoleLogger logger, unsigned int port);
        Socktard(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port);
        Socktard(logtard::ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections);
        ~Socktard();
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