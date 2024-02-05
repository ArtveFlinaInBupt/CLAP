#pragma once

#ifndef CLAP_CFG_CLI_PARSER_H
#  define CLAP_CFG_CLI_PARSER_H

#  include <argparse/argparse.hpp>
#  include <optional>
#  include <string>

#include "util/all.h"

namespace clap {

struct Arguments {
  std::string input_file;
  i32 verbosity;
  bool help;
  bool version;
};

auto args_parse(int argc, char **argv) -> std::optional<Arguments>;

} // namespace clap

#endif // CLAP_CFG_CLI_PARSER_H
