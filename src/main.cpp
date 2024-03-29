#include "../include/socktard.hpp"

int main(void)
{
    auto socket = socktard::Socktard();

    socket.bindSocket();
    socket.listenToConnections();

    SOCKET client_socket;
    void *client_addr = nullptr;

    if (socket.acceptConnection(client_socket, client_addr))
    {
        std::string buffer;
        buffer.resize(2048);
        socket.receiveData(client_socket, &buffer);
        std::cout << socket.getClientIp() << std::endl
                  << buffer << std::endl;

        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body><h1>Hello, world!</h1></body></html>";
        socket.sendData(client_socket, response);
    }
    else
    {
        socket.closeSocket(client_socket);
    }

    socket.closeSocket();

    return 0;
}