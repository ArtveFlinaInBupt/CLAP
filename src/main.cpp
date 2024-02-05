#include "cfg/cli_parser.h"
#include "lexer/file_reader.h"
#include "lexer/lexer.h"
#include "lexer/stat.h"
#include "util/all.h"

#include <iostream>
#include <random>

using namespace clap;

int main(int argc, char **argv) {
  try {
    auto args = args_parse(argc, argv);
    if (!args)
      return EXIT_FAILURE;
    if (args->help || args->version)
      return EXIT_SUCCESS;

    config::set_verbosity(args->verbosity);

    std::cout << std::endl;
    auto lexer = Lexer(args->input_file);
    auto stat = Statistic();
    stat.update_by_file(lexer.file);
    usize line_digits = std::max(len(lexer.file.line_count()) + 1, 4ul);
    usize column_digits = std::max(len(lexer.file.max_line_len()) + 1, 6ul);

    std::cout << std::format(
        "\033[32m{:>{}}:{:<{}} | {}\033[0m\n", "Line", line_digits, "Column",
        column_digits, "          Token Type | Content"
    );

    for (std::optional<Token> token; (token = lexer.next_token());) {
      if (std::holds_alternative<Whitespace>(token->inner()))
        continue;
      stat.update_by_token(token->inner());

      auto [line, column] = lexer.file.lookup_line_and_column(token->start());

      if (std::holds_alternative<LexError>(token->inner())) {
        std::cout << std::format(
            "\033[31mLexError: {} at {}:{}\033[0m\n", token->to_string(),
            line + 1, column + 1
        );
      } else {
        std::cout << std::format(
            "{:>{}}:{:<{}} | {}\n", line + 1, line_digits, column + 1,
            column_digits, token->to_string()
        );
      }
    }

    std::cout << std::endl << stat.to_string() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
