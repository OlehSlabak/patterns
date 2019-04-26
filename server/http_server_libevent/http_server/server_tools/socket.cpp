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

    void Socket::Bind(InetAddressPtr addr)
    {
        logging::DEBUG("Socket::Bind");
        _addr = std::move(addr);
        if (bind(GetHandler(), _addr->GetAddr(), _addr->GetSize()) == -1)
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

    SocketHolderPtr Socket::Accept(bool non_blocking, sockaddr *new_addr, socklen_t *new_addr_len)
    {
        logging::DEBUG("Socket::Accept");
        SocketHolderPtr new_socket(new SocketHolder(accept4(GetHandler(), new_addr, new_addr_len, non_blocking ? SOCK_NONBLOCK : 0)));

        return new_socket;

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
