#include "socket_holder.h"
#include "exceptions.h"

#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <iostream>

namespace Network
{
    DECLARE_RUNTIME_EXCEPTION(SocketHolder)

    SocketHolder::SocketHolder()
        : _handler(invalidSocket) {}


    SocketHolder::SocketHolder(SocketHandle handle)
    : _handler(handle)
    {

        logging::DEBUG("Create " + std::string(__PRETTY_FUNCTION__) +  " object handler[" + std::to_string(_handler) + "]");

        if (_handler == invalidSocket)
        {
            throw SocketHolderException("Invalide socket");
        }
    }

    SocketHolder::~SocketHolder()
    {
        logging::DEBUG("Delete SocketHolder object");
        if (isValid())
        {
            logging::DEBUG("shutdown handler[" + std::to_string(_handler) + "]" );
            shutdown(_handler, SHUT_RDWR);
            close(_handler);
        }
    }

    SocketHandle SocketHolder::GetHandler() const
    {
        return _handler;
    }

    bool SocketHolder::isValid() const
    {
        return _handler != invalidSocket;
    }

    void SocketHolder::Swap(SocketHolder &holder)
    {
        SocketHandle Tmp = _handler;
        _handler = holder._handler;
        holder._handler = Tmp;
    }
}
