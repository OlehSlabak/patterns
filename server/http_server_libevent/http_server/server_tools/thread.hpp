#ifndef THREAD_HPP_INCLUDED
#define THREAD_HPP_INCLUDED
#include "exceptions.h"
#include "non_copyable.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace System
{
    DECLARE_RUNTIME_EXCEPTION(Thread)

    class Thread : public Common::NonCopyable
    {
        public:
            typedef std::function<void ()> _threadFunction;
            typedef std::auto_ptr<_threadFunction> _threadFunctionPointer;
            Thread(_threadFunction const &func);
            ~Thread();
        private:
            _threadFunctionPointer _function;
            pthread_t _handler;
            static void *ThreadFunc(void *prm);
    };
}

#endif // THREAD_HPP_INCLUDED
