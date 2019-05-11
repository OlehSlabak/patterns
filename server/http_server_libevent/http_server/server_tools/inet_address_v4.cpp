#include "inet_address_v4.h"

#include <arpa/inet.h>
#include <iostream>

namespace Network
{
    InetAddressV4 InetAddressV4::CreateFromString(const std::string &addr, unsigned short port)
    {
    logging::INFO("CreateFromString start");
        sockaddr_in sockAdr = { 0 };
        sockAdr.sin_family = AF_INET;
        sockAdr.sin_addr.s_addr = inet_addr(addr.c_str());
        sockAdr.sin_port = htons(port);
        InetAddressV4 _addr(reinterpret_cast<sockaddr const *>(&sockAdr), sizeof(sockAdr));
         logging::INFO("CreateFromString end");
        return _addr;
    }

    InetAddressV4::InetAddressV4(sockaddr const *addr, int size)
    : InetAddress(addr, size) {}

}
