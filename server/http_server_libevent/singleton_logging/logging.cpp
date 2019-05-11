#include "logging.hpp"

using namespace logging;


file_logger::file_logger(const logging_config_t& config)
                        :logger(config)
{
    auto name = config.find("file_name");
    if (name == config.end())
    {
        throw std::runtime_error("No output file rovided");
    }
    file_name_ = name->second;

    reopen_interval_ = std::chrono::seconds(300);
    auto interval = config.find("reopen_interval");
    if (interval != config.end())
    {
        try
        {
            reopen_interval_ = std::chrono::seconds(std::stoul(interval->second));
        }
        catch(...)
        {
            throw std::runtime_error(interval->second + " is not a valid reopen interval");
        }
    }

    reopen();
}

void file_logger::log(const std::string& message, const log_level level)
{
    if (level < LOG_LEVEL)
        return;

    std::string output;
    output.reserve(message.length() + 64);
    output.append(timestamp());
    output.append(colored.find(level)->second);
    output.append(message);;
    output.push_back('\n');
    log(output);
}

void file_logger::log(const std::string& message)
{
    lock_.lock();
  //  std::lock_guard<std::mutex> lock(lock_);
    file_ << message;
    file_.flush();
   lock_.unlock();
    reopen();
}

void file_logger::reopen()
{
    auto now = std::chrono::system_clock::now();

    lock_.lock();

    if (now - last_reopen_ > reopen_interval_)
    {
        last_reopen_ = now;
        try
        {
            file_.close();
        }
        catch(...) {}
        try
        {
            file_.open(file_name_, std::ofstream::out | std::ofstream::app);
            last_reopen_ = std::chrono::system_clock::now();
        }
        catch(std::exception& e)
        {
            try
            {
                file_.close();
            }
            catch(...) {}
            throw e;
        }

    }
    lock_.unlock();
}

void std_out_logger::log(const std::string& message)
{
    std::cout << message;
    std::cout.flush();
}

void std_out_logger::log(const std::string& message, const log_level level)
{
    if(level < LOG_LEVEL)
        return;
      std::string output;
      output.reserve(message.length() + 64);
      output.append(timestamp());
      output.append(levels_.find(level)->second);
      output.append(message);
      output.push_back('\n');
      log(output);
}

logger_factory::logger_factory()
{
    creators_.emplace("", [](const logging_config_t& config)->logger*{return new logger(config);});
    creators_.emplace("std_out", [](const logging_config_t& config)->logger*{return new std_out_logger(config);});
    creators_.emplace("file", [](const logging_config_t& config)->logger*{return new file_logger(config);});
    /* stream_loger*/
}

logger* logger_factory::produce(const logging_config_t& config) const
{
    auto type = config.find("type");
    if (type == config.end())
    {
        throw std::runtime_error("Log factory");
    }

    auto found = creators_.find(type->second);
    if (found != creators_.end())
    {
        return found->second(config);
    }

    throw std::runtime_error("Couldn't produce logger for type: " + type->second);
}


