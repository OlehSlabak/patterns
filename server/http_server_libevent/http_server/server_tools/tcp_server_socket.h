#ifndef TCP_SERVER_SOCKET_H_INCLUDED
#define TCP_SERVER_SOCKET_H_INCLUDED

#include "socket.h"
#include "inet_address.h"

namespace Network
{
    class TCPServerSocket : public Socket
    {
        public:
            TCPServerSocket(InetAddress const &locAddr, int backlog);
    };
}

#endif // TCP_SERVER_SOCKET_H_INCLUDED
