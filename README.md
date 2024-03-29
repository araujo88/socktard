# Socktard

## Introduction

Socktard is a simple C++ network library designed for ease of use and integration. With support for both Unix-based and Windows platforms, it provides a straightforward API for socket communication, including server and client operations. Built on top of the also simple `logtard` logging library, Socktard ensures that your networking applications not only run efficiently but are also well-informed through comprehensive logging.

## Features

- Cross-platform support for Unix and Windows.
- Simplified socket operations such as bind, listen, accept, send, and receive.
- Integration with `logtard` for extensive logging capabilities.
- Exception handling for robust error management.
- HTTP response example included.

## Prerequisites

To use Socktard, you need:

- C++20 compiler
- CMake 3.10 or higher
- Git (for fetching external dependencies)

## External Dependencies

Socktard depends on the `logtard` logging library, which is automatically fetched and built using CMake's FetchContent module.

## Quick Start

1. **Clone the repository:**

   ```bash
   git clone https://github.com/araujo88/socktard.git
   cd socktard
   ```

2. **Build the project:**

   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run the application:**

   After building, you can find the executable in the `build` directory. Run it to start the server and wait for connections.

## Usage Example

The main application demonstrates a simple server that listens for incoming connections, receives a message, prints it, and sends back an HTTP response.

```cpp
#include "socktard.hpp"

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
        socket.receiveData(client_socket, &buffer);
        std::cout << buffer << std::endl;

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world!</h1></body></html>";
        socket.sendData(client_socket, response);
    }
    else
    {
        socket.closeSocket(client_socket);
    }

    socket.closeSocket();

    return 0;
}
```

## Directory Structure

```
socktard/
├── include/       # Header files
├── src/           # Source files
├── CMakeLists.txt # CMake project file
└── README.md      # This file
```

## Contributing

Contributions are welcome! Feel free to open an issue or pull request.

## License

Socktard is open-sourced under the GPL license. See the LICENSE file for details.
