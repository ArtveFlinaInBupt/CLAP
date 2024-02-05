#include "lexer/lexer.h"

#include <cctype>

namespace clap {

Lexer::Lexer(const std::string &file_path): file(file_path) {}

Lexer::Lexer(FileReader &&file) noexcept: file(std::move(file)) {}

std::optional<char> Lexer::peek(usize offset) const {
  if (pos_ + offset >= file.content->size())
    return std::nullopt;
  return file.content->at(pos_ + offset);
}

std::optional<char> Lexer::consume() {
  if (reached_eof())
    return std::nullopt;
  return file.content->at(pos_++);
}

bool Lexer::reached_eof() const {
  return pos_ >= file.content->size();
}

std::optional<Token> Lexer::next_token() {
  usize initial_pos = pos_;

  auto cur_char = consume();
  if (!cur_char)
    return std::nullopt;

  TokenBase token;
  if (isspace(*cur_char)) {
    token = consume_whitespace();
  } else if (cur_char == '/' && peek() == '/') {
    token = consume_inline_comment();
  } else if (cur_char == '/' && peek() == '*') {
    token = consume_block_comment();
  } else if (cur_char == '"') {
    token = consume_string_literal();
  } else if (cur_char == 'L' && peek() == '"') {
    consume();
    token = consume_string_literal();
  } else if (cur_char == '\'') {
    token = consume_character_constant();
  } else if (cur_char == 'L' && peek() == '\'') {
    consume();
    token = consume_character_constant();
  } else if (isalpha(*cur_char) || cur_char == '_') {
    token = consume_identifier_or_keyword();
  } else if (isdigit(*cur_char)) {
    token = consume_number(*cur_char);
  } else if (cur_char == '.' && peek() && isdigit(*peek())) {
    token = consume_number('.');
  } else if (cur_char == '#' || (cur_char == '%' && peek() == ':')) {
    token = consume_preprocessor_command();
  } else if (ispunct(*cur_char)) {
    token = consume_punctuator(*cur_char);
  } else {
    token = LexError(LexErrorType::IllegalCharacter);
  }

  return Token(token, initial_pos, pos_, file.content);
}

TokenBase Lexer::consume_whitespace() {
  while (peek() && isspace(*peek()))
    consume();
  return Whitespace{};
}

TokenBase Lexer::consume_preprocessor_command() {
  consume();
  while (peek() && *peek() != '\n')
    consume();
  return PreprocessorCommand{};
}

TokenBase Lexer::consume_inline_comment() {
  consume();
  while (peek() && *peek() != '\n')
    consume();
  return Comment{};
}

TokenBase Lexer::consume_block_comment() {
  consume();

  while (peek()) {
    if (consume() == '*' && peek() == '/') {
      consume();
      return Comment{};
    }
  }

  return LexError{LexErrorType::UnclosedBlockComment};
}

TokenBase Lexer::consume_string_literal() {
  while (peek()) {
    if (char next_char = *consume(); next_char == '"')
      return StringLiteral{};
    else if (next_char == '\n')
      break;
    else if (next_char == '\\') {
      if (peek() == '"' || peek() == '\\' || peek() == '\n') {
        consume();
      }
    }
  }
  return LexError{LexErrorType::UnclosedStringLiteral};
}

TokenBase Lexer::consume_character_constant() {
  while (peek()) {
    if (char next_char = *consume(); next_char == '\'')
      return Constant{ConstantType::Character};
    else if (next_char == '\n')
      break;
    else if (next_char == '\\') {
      if (peek() == '\'' || peek() == '\\' || peek() == '\n') {
        consume();
      }
    }
  }
  return LexError{LexErrorType::UnclosedCharacterConstant};
}

TokenBase Lexer::consume_identifier_or_keyword() {
  usize identifier_initial_pos = pos_ - 1;
  skip_identifier_or_keyword();

  if (std::string s = file.content->substr(
          identifier_initial_pos, pos_ - identifier_initial_pos
      );
      Keyword::KEYWORDS.contains(s))
    return Keyword{};

  return Identifier{};
}

TokenBase Lexer::consume_number(char first_char) {
  if (first_char == '0') {
#ifdef CLAP_LOG
    log(LogLevel::Debug, std::format("0.{}...", *peek()));
#endif // CLAP_LOG
    if (peek() == 'x' || peek() == 'X') {
      consume();
      return consume_hexadecimal_constant();
    } else if (isdigit(*peek())) {
      return consume_octal_constant();
    } else if (peek() == '.' || peek() == 'e' || peek() == 'E') {
      return consume_float_constant();
    } else if (peek() == 'u' || peek() == 'U' || peek() == 'l' || peek() == 'L') {
      return consume_decimal_constant();
    } else {
      return Constant{ConstantType::Integer};
    }
  } else if (first_char == '.') {
    return consume_float_constant(true);
  } else {
    return consume_decimal_constant();
  }
}

TokenBase Lexer::consume_punctuator(char c) {
  static std::set<char> only_single_char_punctuator = {'(', ')', ',', ';', '?',
                                                       '[', ']', '{', '}', '~'};
  if (only_single_char_punctuator.contains(c))
    return Punctuator{};

  if (c == '!' || c == '^' || c == '/' || c == '*' || c == '=') {
    if (peek() == '=')
      consume();
    return Punctuator{};
  }

  // if (c == '#') {
  //   if (peek() == '#')
  //     consume();
  ////    return Punctuator{};
  // }

  if (c == '&' || c == '|' || c == '+') {
    if (peek() == c || peek() == '=')
      consume();
    return Punctuator{};
  }

  if (c == '.') {
    if (peek() == '.' && peek(1) == '.') {
      consume();
      consume();
    }
    return Punctuator{};
  }

  if (c == '-') {
    if (peek() == '-' || peek() == '=' || peek() == '>') {
      consume();
    }
    return Punctuator{};
  }

  if (c == '<') {
    if (peek() == '=' || peek() == ':' || peek() == '%')
      consume();
    else if (peek() == '<') {
      consume();
      if (peek() == '=')
        consume();
    }
    return Punctuator{};
  }

  if (c == '>') {
    if (peek() == '=')
      consume();
    else if (peek() == '>') {
      consume();
      if (peek() == '=')
        consume();
    }
    return Punctuator{};
  }

  if (c == ':') {
    if (peek() == '>')
      consume();
    return Punctuator{};
  }

  if (c == '%') {
    if (peek() == '=' || peek() == ':' || peek() == '>')
      consume();
    else if (peek() == ':') {
      consume();
      if (peek() == '%' && peek(1) == ':') {
        consume();
        consume();
      }
    }
    return Punctuator{};
  }

  return LexError{LexErrorType::IllegalPunctuator};
}

TokenBase Lexer::consume_hexadecimal_constant() {
  bool has_digits = skip_hexadecimal_digits();
  if (!has_digits)
    return LexError{LexErrorType::IllegalIntegerConstant};
  if (skip_integer_suffix(false))
    return Constant{ConstantType::Integer};
  return LexError{LexErrorType::IllegalIntegerConstant};
}

TokenBase Lexer::consume_decimal_constant() {
  skip_decimal_digits();
  if (peek() == '.' || peek() == 'e' || peek() == 'E')
    return consume_float_constant();
  if (skip_integer_suffix(false))
    return Constant{ConstantType::Integer};
  return LexError{LexErrorType::IllegalIntegerConstant};
}

TokenBase Lexer::consume_octal_constant() {
  skip_octal_digits();
  if (skip_integer_suffix(true))
    return Constant{ConstantType::Integer};
  return LexError{LexErrorType::IllegalIntegerConstant};
}

TokenBase Lexer::consume_float_constant(bool has_decimal_point) {
  if (has_decimal_point) {
    skip_decimal_digits();
  } else if (peek() == '.') {
    consume();
    skip_decimal_digits();
  }
  if (peek() == 'e' || peek() == 'E') {
    if (!skip_exponent_part())
      return LexError{LexErrorType::IllegalFloatConstant};
  }
  if (peek() == '.') {
    skip_all_number_char();
    return LexError{LexErrorType::IllegalFloatConstant};
  }
  if (skip_float_suffix())
    return Constant{ConstantType::Float};
  return LexError{LexErrorType::IllegalFloatConstant};
}

void Lexer::skip_identifier_or_keyword() {
  while (peek() && (isalnum(*peek()) || *peek() == '_'))
    consume();
}

bool Lexer::skip_octal_digits() {
  bool has_digits = false;
  while (peek() && isdigit(*peek()) && *peek() <= '7') {
    consume();
    has_digits = true;
  }
  return has_digits;
}

bool Lexer::skip_decimal_digits() {
  bool has_digits = false;
  while (peek() && isdigit(*peek())) {
    consume();
    has_digits = true;
  }
  return has_digits;
}

bool Lexer::skip_hexadecimal_digits() {
  bool has_digits = false;
  while (peek() && isxdigit(*peek())) {
    consume();
    has_digits = true;
  }
  return has_digits;
}

bool Lexer::skip_integer_suffix(bool allow_floating_point) {
  usize suffix_initial_pos = pos_;
  skip_all_number_char();
  std::string s =
      file.content->substr(suffix_initial_pos, pos_ - suffix_initial_pos);

  usize cnt_u = 0, cnt_l = 0, cnt_U = 0, cnt_L = 0;
  for (char c : s) {
    if (c == 'u')
      cnt_u++;
    else if (c == 'l')
      cnt_l++;
    else if (c == 'U')
      cnt_U++;
    else if (c == 'L')
      cnt_L++;
    else if (c == '.' && !allow_floating_point)
      return false;
  }
  if (cnt_u && cnt_U)
    return false;
  if (cnt_l && cnt_L)
    return false;
  if (cnt_u > 1 || cnt_U > 1 || cnt_l > 2 || cnt_L > 2)
    return false;
  if (s.size() == 3 && (s[1] == 'u' || s[1] == 'U'))
    return false;
  return true;
}

bool Lexer::skip_float_suffix() {
  usize suffix_initial_pos = pos_;
  skip_all_number_char();
  std::string s =
      file.content->substr(suffix_initial_pos, pos_ - suffix_initial_pos);
  return s.empty() || s == "f" || s == "l" || s == "F" || s == "L";
}

bool Lexer::skip_exponent_part() {
  if (peek() == 'e' || peek() == 'E') {
    consume();

    bool valid = true;
    if (peek() == '+' || peek() == '-')
      consume();
    else if (peek() == '.') {
      valid = false;
      consume();
    }
    return skip_decimal_digits() && valid;
  }

  return false;
}

void Lexer::skip_all_number_char() {
  while (isalnum(*peek()) || *peek() == '.')
    consume();
}

} // namespace clap
