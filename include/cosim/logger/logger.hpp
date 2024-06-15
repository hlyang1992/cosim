
#ifndef LIBCOSIM_LOGGER_HPP
#define LIBCOSIM_LOGGER_HPP

#include <fmt/core.h>

namespace cosim::log
{

enum class level : int
{
    trace,
    debug,
    info,
    warn,
    err,
    off
};

void set_logging_level(level lvl);

void log(level lvl, const std::string& msg);

template<typename... Args>
void trace(fmt::format_string<Args...> fmt, Args&&... args)
{
    log(level::trace, fmt::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void debug(fmt::format_string<Args...> fmt, Args&&... args)
{
    log(level::debug, fmt::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void info(fmt::format_string<Args...> fmt, Args&&... args)
{
    log(level::info, fmt::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void warn(fmt::format_string<Args...> fmt, Args&&... args)
{
    log(level::warn, fmt::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void err(fmt::format_string<Args...> fmt, Args&&... args)
{
    log(level::err, fmt::format(fmt, std::forward<Args>(args)...));
}

} // namespace cosim::log


#endif // LIBCOSIM_LOGGER_HPP
