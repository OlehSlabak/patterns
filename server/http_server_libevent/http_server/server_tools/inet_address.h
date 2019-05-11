#ifndef INET_ADDRESS_H_INCLUDED
#define INET_ADDRESS_H_INCLUDED

#include <sys/socket.h>
#include "exceptions.h"
#include "non_copyable.h"

namespace Network
{
    class InetAddress : private Common::NonCopyable
    {
        public:
            InetAddress(InetAddress const &addr);
            InetAddress& operator = (InetAddress const &addr);
            virtual ~InetAddress();
            sockaddr const* GetAddr() const;
            int GetSize() const;
            void Swap(InetAddress &addr);
            static InetAddress CreateFromSockAddr(sockaddr *addr, int size);
        protected:
            InetAddress(sockaddr const *addr, int size);
        private:
            sockaddr *_addr;
            int _size;
            void Init(sockaddr const *addr, int size);
    };
}


#endif // INET_ADDRESS_H_INCLUDED
