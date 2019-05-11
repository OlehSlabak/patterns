#include "socket_tuner.h"

#include "exceptions.h"

#include <errno.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

namespace Network
{
    DECLARE_RUNTIME_EXCEPTION(SocketTuner)

    SocketTuner::SocketTuner(SocketHandle handler)
        : _handle(handler) {}

    void SocketTuner::SetReuseAddr(bool reuse)
    {
        int on = reuse ? 1 : 0;
        CheckResult(setsockopt(_handle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)), -1, "Failed to set SO_REUSEADDR", errno);
    }

    void SocketTuner::SetCorking(bool useCorking)
    {
        int on = useCorking ? 1 : 0;
        CheckResult(setsockopt(_handle, IPPROTO_TCP, TCP_CORK, &on, sizeof(on)), -1, "Failed to set TCP_CORK", errno);
    }

    void SocketTuner::SetNoDelay(bool noDelay)
    {
        int on = noDelay ? 1 : 0;
        CheckResult(setsockopt(_handle, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)), -1, "Failed to set TCP_NODELAY", errno);
    }

    void SocketTuner::CheckResult(int code, int invalideCode, char const *msg, int errNo) const
    {

        if (code != invalideCode)
            return
        throw SocketTunerException(msg, errNo);
    }
}
