#ifndef INET_ADDRESS_H_INCLUDED
#define INET_ADDRESS_H_INCLUDED

#include <sys/socket.h>
#include <memory>

namespace Network
{
    class InetAddress
    {
        public:
            virtual ~InetAddress();
            sockaddr const* GetAddr() const;
            int GetSize() const;
        protected:
            InetAddress(sockaddr const *addr, int size);
        private:
            sockaddr *_addr;
            int _size;
            void Init(sockaddr const *addr, int size);
    };
    typedef std::unique_ptr<InetAddress> InetAddressPtr;
}


#endif // INET_ADDRESS_H_INCLUDED
