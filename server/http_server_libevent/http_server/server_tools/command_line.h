#ifndef COMMAND_LINE_H_INCLUDED
#define COMMAND_LINE_H_INCLUDED

#include <string>
#include <unordered_map>
#include <sstream>

namespace Common
{
    class CommandLine
    {
        public:
            CommandLine(int argc, const char **argv);
            std::string GetProgramName() const;
            std::string GetStrParameter(const std::string& name) const;
            template <typename T> T GetParameter(const std::string& name) const
            {
                std::stringstream stream;
                stream << GetStrParameter(name);
                T value = T();
                stream >> value;
                return value;
            }
        private:
            std::string _programName;
            typedef std::unordered_map<std::string, std::string> _paramPool;
            _paramPool _params;
    };
}

#endif // COMMAND_LINE_H_INCLUDED
