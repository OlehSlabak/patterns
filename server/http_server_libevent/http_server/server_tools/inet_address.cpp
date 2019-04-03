#include "inet_address.h"

#include "exceptions.h"
#include <string>
#include <cstring>

namespace Network
{
    DECLARE_RUNTIME_EXCEPTION(InetAddress)

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
