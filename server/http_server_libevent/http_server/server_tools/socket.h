#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include "socket_holder.h"
#include "inet_address.h"

#include <sys/socket.h>


namespace Network
{
    class Socket : public SocketHolder
    {
        public:
            enum Type {TCP, UDP};
            Socket(Type type, bool non_blocking);
            void Bind(InetAddress const &addr);
            void Listen(int backlog);
            void Accept(SocketHolder *newSocket, bool nonBlocking,
                        sockaddr *newAddr, socklen_t *newAddrSize);
        private:
            static int GetSockType(Type type);
        protected:
            InetAddressPtr _addr;
    };

}

#endif // SOCKET_H_INCLUDED
