#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

/* logging singleton */

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <ctime>
#include <cstdlib>

namespace logging
{
    enum class log_level : uint8_t { TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };
    struct enum_hasher {
        template <typename T> std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    const std::unordered_map<log_level, std::string, logging::enum_hasher> uncolored
    {
        {log_level::TRACE, "[TRACE]"},{log_level::DEBUG, "[DEBUG]"},{log_level::INFO, "[INFO]"},
        {log_level::WARN, "[WARN]"},{log_level::ERROR, "[ERROR]"}
    };

    const std::unordered_map<log_level, std::string, enum_hasher> colored
    {
      {log_level::ERROR, " \x1b[31;1m[ERROR]\x1b[0m "}, {log_level::WARN, " \x1b[33;1m[WARN]\x1b[0m "},
      {log_level::INFO, " \x1b[32;1m[INFO]\x1b[0m "}, {log_level::DEBUG, " \x1b[34;1m[DEBUG]\x1b[0m "},
      {log_level::TRACE, " \x1b[37;1m[TRACE]\x1b[0m "}
    };

    #if defined(LOGGING_LEVEL_ALL) || defined(LOGGING_LEVEL_TRACE)
        constexpr log_level LOG_LEVEL = log_level::TRACE;
    #elif defined(LOGGING_LEVEL_DEBUG)
        constexpr log_level LOG_LEVEL = log_level::DEBUG;
    #elif defined(LOGGING_LEVEL_WARN)
        constexpr log_level LOG_LEVEL = log_level::WARN;
    #elif defined(LOGGING_LEVEL_ERROR)
        constexpr log_level LOG_LEVEL = log_level::ERROR;
    #else
        constexpr log_level LOG_LEVEL = log_level::INFO;
    #endif

    inline std::string timestamp()
    {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm gmt{};
        gmtime_r(&time, &gmt);
        std::chrono::duration<double> fractional_seconds =
              (tp - std::chrono::system_clock::from_time_t(time)) + std::chrono::seconds(gmt.tm_sec);

        std::string buffer("year/mo/dy hr:mn:sc.xxxxxx");
        sprintf(&buffer.front(), "%04d/%02d/%02d %02d:%02d:%09.6f", gmt.tm_year + 1900, gmt.tm_mon + 1,
                                  gmt.tm_mday, gmt.tm_hour, gmt.tm_min, fractional_seconds.count());
        return buffer;
    }

    using logging_config_t = std::unordered_map<std::string, std::string>;

    class logger {
        public:
            logger() = delete;
            logger(const logging_config_t& config) {};
            virtual ~logger() {};
            virtual void log(const std::string&, const log_level) {};
            virtual void log(const std::string&) {};
        protected:
            std::mutex lock_;
    };

    class std_out_logger : public logger {
        public:
            std_out_logger() = delete;
            std_out_logger(const logging_config_t& config) : logger(config), levels_(config.find("color") != config.end() ? colored : uncolored) {}
            virtual void log(const std::string& message, const log_level level);
            virtual void log(const std::string& message);

        protected:
            const std::unordered_map<log_level, std::string, enum_hasher> levels_;

    };

    class file_logger : public logger {
        public:
            file_logger() = delete;
            file_logger(const logging_config_t& config);
            virtual void log(const std::string& message, const log_level level);
            virtual void log(const std::string& messages);
        protected:
            void reopen();
            std::string file_name_;
            std::ofstream file_;

            std::chrono::seconds reopen_interval_;
            std::chrono::system_clock::time_point last_reopen_;
    };

    using logger_creator = logger *(*)(const logging_config_t&);

    class logger_factory {
        public:
            logger_factory();
            logger* produce(const logging_config_t& config) const;
        protected:
            std::unordered_map<std::string, logger_creator> creators_;
    };

    inline logger_factory& get_factory()
    {
        static logger_factory factory_singleton{};
        return factory_singleton;
    }

    inline logger& get_logger(const logging_config_t& config = { {"type", "std_out"}, {"color", ""} })
    {
        static std::unique_ptr<logger> singleton(get_factory().produce(config));
        return *singleton;
    }

    inline void configure(const logging_config_t& config)
    {
        get_logger(config);
    }

    inline void log(const std::string& message, const log_level level)
    {
        get_logger().log(message, level);
    }

    inline void log(const std::string& message)
    {
        get_logger().log(message);
    }

    inline void TRACE(const std::string& message)
    {
        get_logger().log(message, log_level::TRACE);
    }

    inline void DEBUG(const std::string& message)
    {
        get_logger().log(message, log_level::DEBUG);
    }

    inline void INFO(const std::string& message)
    {
        get_logger().log(message, log_level::INFO);
    }

    inline void WARN(const std::string& message)
    {
        get_logger().log(message, log_level::WARN);
    }

    inline void ERROR(const std::string& message)
    {
        get_logger().log(message, log_level::ERROR);
    }
}
#endif // LOGGING_HPP_INCLUDED
