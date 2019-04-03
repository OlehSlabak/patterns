#ifndef NON_COPYABLE_H_INCLUDED
#define NON_COPYABLE_H_INCLUDED

namespace Common
{
    class NonCopyable
    {
        protected:
            NonCopyable() {}
            ~NonCopyable(){}
        private:
            NonCopyable(const NonCopyable &) = delete;
            NonCopyable(const NonCopyable &&) = delete;
            NonCopyable& operator= (const NonCopyable &) = delete;
            NonCopyable& operator= (const NonCopyable &&) = delete;
    };
}


#endif // NON_COPYABLE_H_INCLUDED
