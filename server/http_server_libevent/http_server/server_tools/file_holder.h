#ifndef FILE_HOLDER_H_INCLUDED
#define FILE_HOLDER_H_INCLUDED

#include "exceptions.h"

#include "non_copyable.h"
#include <string>

namespace System
{
    DECLARE_RUNTIME_EXCEPTION(FileHolder)

    class FileHolder : private Common::NonCopyable
    {
        public:
            FileHolder(const std::string &fileName);
            virtual ~FileHolder();
            void Open(const std::string &fileName);
            int GetHandle() const;
            int GetSize() const;
            int Detach();
        private:
            int _handle;
            int _size;
    };
}


#endif // FILE_HOLDER_H_INCLUDED
