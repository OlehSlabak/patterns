#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdexcept>
#include "../../singleton_logging/logging.hpp"

#define DECLARE_RUNTIME_EXCEPTION(class_name_) \
    class class_name_##Exception \
            : public std::runtime_error \
    { \
        public: \
            class_name_##Exception(std::string const &msg, int code = 0, const logging::log_level level = logging::log_level::ERROR) \
               : std::runtime_error(msg) \
               , _code(code) \
              { \
                logging::log(msg, level); \
              } \
              virtual int GetCode() const \
              { \
                return _code; \
              } \
        private: \
            int _code; \
    };

#endif // EXCEPTIONS_H_INCLUDED
