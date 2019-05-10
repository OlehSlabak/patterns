#include "thread.hpp"
#include <errno.h>
#include <assert.h>

namespace System
{
    Thread::Thread(_threadFunction const &func) : _function(new _threadFunction(func))
    {
        if (pthread_create(&_handler, 0, &Thread::ThreadFunc, _function.get()) == -1)
            throw ThreadException("Failed to create thread", errno);
    }

    Thread::~Thread()
    {
        int res = pthread_join(_handler, 0);
        assert(res != -1 && "Failed to join thread");
    }

    void* Thread::ThreadFunc(void *prm)
    {
        (*reinterpret_cast<_threadFunction *>(prm))();
        return 0;
    }
}
