#pragma once

#ifndef CLAP_UTIL_TIME_H
#  define CLAP_UTIL_TIME_H

#  include <chrono>
#  include <string>

namespace clap {

inline std::string get_current_time_str() {
  auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now()
  );
  return std::format("{:%m-%d %T}", now);
}

} // namespace clap

#endif // CLAP_UTIL_TIME_H
