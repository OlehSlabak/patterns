#include "socket.h"
#include "inet_address.h"
#include "exceptions.h"

#include <errno.h>

namespace Network
{
    DECLARE_RUNTIME_EXCEPTION(Socket)

    Socket::Socket(Type type, bool non_blocking)
        : SocketHolder(socket(AF_INET, GetSockType(type) | (non_blocking ? SOCK_NONBLOCK : 0), 0))
        {}

    void Socket::Bind(InetAddress const &addr)
    {
        logging::DEBUG("Socket::Bind");
        if (bind(GetHandler(), addr.GetAddr(), addr.GetSize()) == -1)
        {
            throw SocketException("Failed to bind socket", errno);
        }
    }

    void Socket::Listen(int backlog)
    {
        logging::DEBUG("Socket::Listen");
        if (listen(GetHandler(), backlog) == -1)
        {
            throw SocketException("Failed to listen socket", errno);
        }
    }

    void Socket::Accept(SocketHolder *newSocket, bool nonBlocking, sockaddr *newAddr, socklen_t *newAddrSize)
    {
        if (!newSocket)
          throw SocketException("Invalid input parameter");
        if (newSocket->isValid())
          throw SocketException("Input holder must be not initialized");
        SocketHolder NewSocket(accept4(GetHandler(), newAddr, newAddrSize, nonBlocking ? SOCK_NONBLOCK : 0));
        newSocket->Swap(NewSocket);
    }

    int Socket::GetSockType(Type type)
    {
        logging::DEBUG("Socket::GetSockType");
        switch (type)
        {
            case TCP:
                return SOCK_STREAM;
            case UDP:
                return SOCK_DGRAM;
            default:
                break;
        }
        throw SocketException("Unknow socket type");
    }


}
