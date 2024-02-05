#pragma once

#ifndef CLAP_LEXER_LEXER_H
#  define CLAP_LEXER_LEXER_H

#  include "file_reader.h"
#  include "token.h"

#  include <functional>

namespace clap {

class Lexer {
  usize pos_{};

public:
  FileReader file;

  explicit Lexer(const std::string &file_path);

  Lexer(const FileReader &file) = delete; // no need to impl

  explicit Lexer(FileReader &&file) noexcept;

  Lexer(const Lexer &rhs) = delete;

  [[nodiscard]] std::optional<Token> next_token();

  [[nodiscard]] std::optional<char> peek(usize offset = 0) const;

  std::optional<char> consume();

  [[nodiscard]] bool reached_eof() const;

  [[nodiscard]] TokenBase consume_whitespace();

  [[nodiscard]] TokenBase consume_preprocessor_command();

  [[nodiscard]] TokenBase consume_inline_comment();

  [[nodiscard]] TokenBase consume_block_comment();

  [[nodiscard]] TokenBase consume_string_literal();

  [[nodiscard]] TokenBase consume_character_constant();

  [[nodiscard]] TokenBase consume_identifier_or_keyword();

  [[nodiscard]] TokenBase consume_number(char first_char);

  [[nodiscard]] TokenBase consume_punctuator(char first_char);

  TokenBase consume_octal_constant();

  TokenBase consume_decimal_constant();

  TokenBase consume_hexadecimal_constant();

  TokenBase consume_float_constant(bool has_decimal_point = false);

  void skip_identifier_or_keyword();

  bool skip_octal_digits();

  bool skip_decimal_digits();

  bool skip_hexadecimal_digits();

  bool skip_integer_suffix(bool allow_floating_point);

  bool skip_float_suffix();

  bool skip_exponent_part();

  void skip_all_number_char();
};

} // namespace clap

#endif // CLAP_LEXER_LEXER_H
