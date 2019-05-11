#include "inet_address.h"
#include <string>
#include <cstring>

namespace Network
{
    DECLARE_RUNTIME_EXCEPTION(InetAddress)

    InetAddress::InetAddress(InetAddress const &addr)
    : _addr(0)
    , _size(0)
    {
        Init(addr.GetAddr(), addr.GetSize());
    }

    InetAddress& InetAddress::operator = (InetAddress const &addr)
    {
        InetAddress tmp(addr);
        Swap(tmp);
        return *this;
    }

    InetAddress::InetAddress(sockaddr const *addr, int size)
    : _addr(0)
    , _size(0)
    {
        Init(addr, size);
    }

    InetAddress::~InetAddress()
    {
        logging::DEBUG("Delete InetAddress object");
        ::operator delete (_addr);
    }

    sockaddr const* InetAddress::GetAddr() const
    {
        return _addr;
    }


    void InetAddress::Swap(InetAddress &addr)
    {

        std::swap(addr._addr, _addr);
        std::swap(addr._size, _size);

     /*   sockaddr *tmpAddr = addr._addr;
        addr._addr = _addr;
        _addr = tmpAddr;
        int tmpSize = addr._size;
        addr._size = _size;
        _size = tmpSize; */
    }

    InetAddress InetAddress::CreateFromSockAddr(sockaddr *addr, int size)
    {
        InetAddress _addr(addr, size);
        return _addr;
    }

    int InetAddress::GetSize() const
    {
        return _size;
    }

    void InetAddress::Init(sockaddr const *addr, int size)
    {
        logging::DEBUG("Create InetAddress object");

        if (!addr || !size)
        {
          throw InetAddressException("Invalid address");
        }

        _addr = reinterpret_cast<sockaddr *> (::operator new(size));
        _size = size;
        memcpy(_addr, addr, _size);
    }
}
