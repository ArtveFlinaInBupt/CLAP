#include "cfg/cli_parser.h"

#include "util/constants.h"

#include <argparse/argparse.hpp>
#include <format>
#include <optional>

namespace clap {

// clang-format off
void print_version() {
  std::cout << BOLD << PROJECT << " " << VERSION << RESET << '\n'
            << "Copyright (C) 2023 " << AUTHOR << '\n'
            << "Source available at " << REPO_LINK << std::endl;
}

void print_help(const std::string &name) {
  std::cout << BOLD << UNDERLINE << "Usage:" << RESET << " " << BOLD << name << RESET << " [OPTIONS] [FILE]\n\n"
            << BOLD << UNDERLINE << "Arguments:" << RESET << "\n" << "  [FILE]\n\n"
            << BOLD << UNDERLINE << "Options:" << RESET << "\n"
            << "  " << BOLD << "-h, --help" << RESET << "       Print help\n"
            << "  " << BOLD << "-V, --version" << RESET << "    Print version and exit\n"
            << "  " << BOLD << "-v, --verbosity" << RESET << "  Set verbosity level [possible values: 0, 1, 2, " << UNDERLINE << "3" << RESET << ", 4]" << std::endl;
}

// clang-format on

auto args_parse(int argc, char **argv) -> std::optional<Arguments> {
  Arguments args{};

  argparse::ArgumentParser program(
      std::string(PROJECT), std::string(VERSION),
      argparse::default_arguments::none
  );

  program.add_argument("-h", "--help")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-V", "--version")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-v", "--verbosity")
      .default_value(static_cast<int>(3))
      .nargs(1)
      .scan<'i', int>();

  program.add_argument("file").default_value("");

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    return std::nullopt;
  } catch (const std::invalid_argument &err) {
    std::cerr << err.what() << std::endl;
    return std::nullopt;
  }

  args.verbosity = program.get<int>("--verbosity");
  if ((args.help = program.get<bool>("--help")))
    print_help(argv[0]);
  else if ((args.version = program.get<bool>("--version")))
    print_version();
  else if ((args.input_file = program.get<std::string>("file")).empty()) {
    print_help(argv[0]);
    return std::nullopt;
  }

  return args;
}

} // namespace clap
