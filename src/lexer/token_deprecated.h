#pragma once

#ifndef CLAP_LEXER_TOKEN_H
#  define CLAP_LEXER_TOKEN_H

#  include "util/all.h"

namespace clap {

enum class TokenType : u8 {
  Keyword,
  Identifier,
  Constant,
  StringLiteral,
  Punctuator,
  // Non-language tokens
  Comment,
  Whitespace,
  LexError,
};

enum class ConstantType : u8 {
  Integer,
  Float,
  Character,
};

enum class LexErrorType : u8 {
  InvalidCharacter,
  InvalidFloatConstant,
  InvalidIntegerConstant,
  UnclosedBlockComment,
  UnclosedCharacterConstant,
  UnclosedStringLiteral,
};

class Token {
  usize start_{}, end_{};
  std::shared_ptr<std::string> src_{};
  TokenType type_{};

protected:
  Token(
      TokenType type, usize start, usize end,
      const std::shared_ptr<std::string> &src
  );

public:
  virtual ~Token() = default;

  [[nodiscard]] TokenType inner() const noexcept;

  [[nodiscard]] usize start() const noexcept;

  [[nodiscard]] usize end() const noexcept;

  [[nodiscard]] usize len() const noexcept;

  [[nodiscard]] virtual std::string to_string() const;
};

class Keyword final : public Token {
public:
  Keyword(usize start, usize end, const std::shared_ptr<std::string> &src);

  ~Keyword() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class Identifier final : public Token {
public:
  Identifier(usize start, usize end, const std::shared_ptr<std::string> &src);

  ~Identifier() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class Constant final : public Token {
  ConstantType type_{};

public:
  Constant(
      usize start, usize end, const std::shared_ptr<std::string> &src,
      ConstantType type
  );

  ~Constant() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class StringLiteral final : public Token {
public:
  StringLiteral(
      usize start, usize end, const std::shared_ptr<std::string> &src
  );

  ~StringLiteral() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class Punctuator final : public Token {
public:
  Punctuator(usize start, usize end, const std::shared_ptr<std::string> &src);

  ~Punctuator() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class Comment final : public Token {
public:
  Comment(usize start, usize end, const std::shared_ptr<std::string> &src);

  ~Comment() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class Whitespace final : public Token {
public:
  Whitespace(usize start, usize end, const std::shared_ptr<std::string> &src);

  ~Whitespace() override = default;

  [[nodiscard]] std::string to_string() const override;
};

class LexError final : public Token {
  LexErrorType type_{};

public:
  LexError(
      usize start, usize end, const std::shared_ptr<std::string> &src,
      LexErrorType type
  );

  ~LexError() override = default;

  [[nodiscard]] std::string to_string() const override;
};

} // namespace clap

#endif // CLAP_LEXER_TOKEN_H
