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
            explicit SocketHolder(SocketHandle handle);
            virtual ~SocketHolder();
            SocketHandle GetHandler() const;

        private:
            bool isValid() const;
        SocketHandle _handler;
    };

    typedef std::unique_ptr<SocketHolder> SocketHolderPtr;
}

#endif // SOCKET_HOLDER_H_INCLUDED
