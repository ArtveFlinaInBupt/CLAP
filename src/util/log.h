#pragma once

#ifndef CLAP_UTIL_LOG_H
#  define CLAP_UTIL_LOG_H
#  define CLAP_LOG

#  include "cfg/config.h"
#  include "util/constants.h"
#  include "util/time.h"

#  include <chrono>
#  include <format>
#  include <iostream>
#  include <string>

namespace clap {

enum class LogLevel : int {
  Debug = 0,
  Info = 1,
  Warning = 2,
  Error = 3,
};

inline void log(LogLevel level, const std::string &message) {
  if (static_cast<int>(level) < 4 - config::verbosity)
    return;

  std::string time_display =
      std::format("{}[{}]{}", GRAY, get_current_time_str(), RESET);

  std::string message_display;
  switch (level) {
    case LogLevel::Debug:
      message_display = std::format("{}[DEBUG]{}{}", MAGENTA, message, RESET);
      break;
    case LogLevel::Info:
      message_display = std::format("[INFO]{}", message);
      break;
    case LogLevel::Warning:
      message_display = std::format("{}[WARNING]{}{}", YELLOW, message, RESET);
      break;
    case LogLevel::Error:
      message_display = std::format("{}[ERROR]{}{}", RED, message, RESET);
      break;
    default:
      throw std::runtime_error("Unknown log level");
  }

  std::cerr << std::format("{}{}", time_display, message_display) << std::endl;
}

} // namespace clap

#endif // CLAP_UTIL_LOG_H
