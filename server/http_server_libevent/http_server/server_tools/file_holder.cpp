#include "file_holder.h"

#include <fcntl.h>
#include <unistd.h>

namespace System
{
    FileHolder::FileHolder(const std::string &fileName)
    : _handle(-1), _size(0)
    {
        Open(fileName);
    }

    FileHolder::~FileHolder()
    {
        if (_handle != -1)
            close(_handle);
    }

    void FileHolder::Open(const std::string &fileName)
    {
        logging::DEBUG("Open file" + fileName);
        int newSize = 0;
        int newHandler = open(fileName.c_str(), 0);

        if (newHandler == -1)
            throw FileHolderException("Failed to open file " + fileName);

        newSize = lseek(newHandler, 0, SEEK_SET);

        if (newSize == -1 || (newSize = lseek(newHandler, 0, SEEK_END)) == -1 || lseek(newHandler, 0, SEEK_SET) == -1)
        {
            close(newHandler);
            throw FileHolderException("Failed to set file position");
        }

        if (_handle != -1)
        {
            close(_handle);
        }
        _handle = newHandler;
        _size = newSize;

    }

    int FileHolder::GetHandle() const
    {
        return _handle;
    }

    int FileHolder::GetSize() const
    {
        return _size;
    }

    int FileHolder::Detach()
    {
        logging::DEBUG("Detach");
        int ret = _handle;
        _handle = -1;
        _size = -1;
        return ret;
    }
}
