#ifndef SOCKET_HOLDER_H_INCLUDED
#define SOCKET_HOLDER_H_INCLUDED

#include "non_copyable.h"
#include "my_types.h"
#include <cstdlib>
#include <memory>

namespace Network
{
    class SocketHolder : private Common::NonCopyable
    {
        public:
            SocketHolder();
            SocketHolder(SocketHandle handle);
            virtual ~SocketHolder();
            SocketHandle GetHandler() const;
            bool isValid() const;
            void Swap(SocketHolder &holder);
        private:
        SocketHandle _handler;
    };
}

#endif // SOCKET_HOLDER_H_INCLUDED
