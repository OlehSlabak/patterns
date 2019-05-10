#include "command_line.h"
#include "exceptions.h"
#include <string.h>

namespace Common
{
    DECLARE_RUNTIME_EXCEPTION(CommandLine)

    CommandLine::CommandLine(int argc, char const **argv)
    {
        if (!argc || !argv)
            throw CommandLineException("Invalide input parameters");

        _programName = argv[0];

        for (int i = 1; i < argc; ++i)
        {
            char const *delimiter = strstr(argv[i], ":");
            if (!delimiter)
                throw CommandLineException("Invalide paramenters");

            _params[std::string(argv[i], delimiter - argv[i])] = delimiter + 1;
        }
    }

    std::string CommandLine::GetProgramName() const
    {
        return _programName;
    }

    std::string CommandLine::GetStrParameter(std::string const &name) const
    {
        _paramPool::const_iterator iter = _params.find(name);
        if (iter == _params.end())
            throw CommandLineException("Parameter not found");
        return iter->second;
    }
}
