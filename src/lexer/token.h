#pragma once

#ifndef CLAP_LEXER_TOKEN_H
#  define CLAP_LEXER_TOKEN_H

#  include "util/all.h"

#  include <string_view>

using namespace std::literals::string_view_literals;

namespace clap {

// enum class TokenBase : u8 {
//   Keyword,
//   Identifier,
//   Constant,
//   StringLiteral,
//   Punctuator,
//   // Non-language tokens
//   Comment,
//   Whitespace,
//   LexError,
// };

enum class ConstantType : u8 {
  Integer,
  Float,
  Character,
};

enum class LexErrorType : u8 {
  IllegalCharacter,
  IllegalFloatConstant,
  IllegalIntegerConstant,
  IllegalPunctuator,
  UnclosedBlockComment,
  UnclosedCharacterConstant,
  UnclosedStringLiteral,
};

struct Keyword {
  const inline static std::set<std::string_view> KEYWORDS = {
      "auto"sv,       "break"sv,    "case"sv,   "char"sv,     "const"sv,
      "continue"sv,   "default"sv,  "do"sv,     "double"sv,   "else"sv,
      "enum"sv,       "extern"sv,   "float"sv,  "for"sv,      "goto"sv,
      "if"sv,         "int"sv,      "long"sv,   "register"sv, "return"sv,
      "short"sv,      "signed"sv,   "sizeof"sv, "static"sv,   "struct"sv,
      "switch"sv,     "typedef"sv,  "union"sv,  "unsigned"sv, "void"sv,
      "volatile"sv,   "while"sv, // C89
      "inline"sv,     "restrict"sv, "_Bool"sv,  "_Complex"sv,
      "_Imaginary"sv, // C99
                      // "_Alignas"sv,      "_Alignof"sv,
                      // "_Atomic"sv,       "_Generic"sv,
                      // "_Noreturn"sv,     "_Static_assert"sv,
                      // "_Thread_local"sv, // C11
                      // "alignas"sv,       "alignof"sv,
                      // "bool"sv,          "constexpr"sv,
                      // "false"sv,         "nullptr"sv,
                      // "static_assert"sv, "thread_local"sv,
                      // "true"sv,          "typeof"sv,
                      // "typeof_unqual"sv, "_BitInt"sv,
                      // "_Decimal128"sv,   "_Decimal32"sv,
                      // "_Decimal64"sv, // C23
  };
};

struct Identifier {};

struct Constant {
  ConstantType type_{};

  explicit Constant(ConstantType type);
};

struct StringLiteral {};

struct Punctuator {};

struct PreprocessorCommand {};

struct Comment {};

struct Whitespace {};

struct LexError {
  LexErrorType type_{};

  explicit LexError(LexErrorType type);
};

using TokenBase = std::variant<
    Keyword, Identifier, Constant, StringLiteral, Punctuator,
    PreprocessorCommand, Comment, Whitespace, LexError>;

class Token {
  std::shared_ptr<std::string> src_{};
  usize start_{}, end_{};
  TokenBase token_{};

public:
  Token(
      TokenBase token, usize start, usize end,
      const std::shared_ptr<std::string> &src
  );

  [[nodiscard]] TokenBase inner() const noexcept;

  [[nodiscard]] usize start() const noexcept;

  [[nodiscard]] usize end() const noexcept;

  [[nodiscard]] usize len() const noexcept;

  [[nodiscard]] std::string get_name() const;

  [[nodiscard]] std::string to_string() const;
};

} // namespace clap

#endif // CLAP_LEXER_TOKEN_H
