#ifndef INET_ADDRESS_V4_H_INCLUDED
#define INET_ADDRESS_V4_H_INCLUDED

#include "inet_address.h"
#include <string>

namespace Network
{
    class InetAddressV4 : public InetAddress
    {
        public:
            static InetAddressV4 CreateFromString(const std::string &addr, unsigned short port);
        private:
            InetAddressV4(sockaddr const *addr, int size);
    };
}

#endif // INET_ADDRESS_V4_H_INCLUDED
