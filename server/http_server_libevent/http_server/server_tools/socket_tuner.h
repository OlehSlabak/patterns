#ifndef SOCKET_TUNER_H_INCLUDED
#define SOCKET_TUNER_H_INCLUDED

#include "my_types.h"

namespace Network
{
    class SocketTuner
    {
        public:
            SocketTuner(SocketHandle handle);
            void SetReuseAddr(bool reuse);
            void SetCorking(bool useCorking);
            void SetNoDelay(bool noDelay);
        private:
            SocketHandle _handle;
            void CheckResult(int code, int invalideCode, char const *msg, int errNo) const;
    };
}

#endif // SOCKET_TUNER_H_INCLUDED
