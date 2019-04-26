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
            void Bind(InetAddressPtr addr);
            void Listen(int backlog);
            SocketHolderPtr Accept(bool non_blocking, sockaddr *new_addr = 0, socklen_t *new_addr_size = 0);
        private:
            static int GetSockType(Type type);
        protected:
            InetAddressPtr _addr;
    };

}

#endif // SOCKET_H_INCLUDED
