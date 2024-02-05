#pragma once

#ifndef CLAP_UTIL_CONSTANTS_H
#  define CLAP_UTIL_CONSTANTS_H

#  include <set>
#  include <string_view>

namespace clap {

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view BOLD = "\033[1m";
constexpr std::string_view UNDERLINE = "\033[4m";
constexpr std::string_view RED = "\033[31m";
constexpr std::string_view GREEN = "\033[32m";
constexpr std::string_view YELLOW = "\033[33m";
constexpr std::string_view BLUE = "\033[34m";
constexpr std::string_view MAGENTA = "\033[35m";
constexpr std::string_view CYAN = "\033[36m";
constexpr std::string_view WHITE = "\033[37m";
constexpr std::string_view GRAY = "\033[90m";

constexpr std::string_view PROJECT = "CLAP";
constexpr std::string_view VERSION = "0.1.0";
#  define AUTHOR    "fa_555 <fa_555@foxmail.com>"
#  define REPO_LINK "UNKNOWN"

} // namespace clap

#endif // CLAP_UTIL_CONSTANTS_H
