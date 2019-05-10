#include "tcp_server_socket.h"
#include "socket_tuner.h"


namespace Network
{
    TCPServerSocket::TCPServerSocket(InetAddress const &locAddr, int backlog)
        : Socket(Socket::TCP, true)
        {
            SocketTuner Tuner(GetHandler());
            Tuner.SetReuseAddr(true);
            Bind(std::move(locAddr));
            Listen(backlog);
        }

}
