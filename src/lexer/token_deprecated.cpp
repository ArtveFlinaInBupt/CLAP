#include "lexer/token.h"

namespace clap {

Token::Token(
    TokenType type, usize start, usize end,
    const std::shared_ptr<std::string> &src
):
    start_(start),
    end_(end), src_(src), type_(type) {}

TokenType Token::inner() const noexcept {
  return type_;
}

usize Token::start() const noexcept {
  return start_;
}

usize Token::end() const noexcept {
  return end_;
}

usize Token::len() const noexcept {
  return end_ - start_;
}

std::string Token::to_string() const {
  throw std::runtime_error("Not implemented");
}

Keyword::Keyword(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::Keyword, start, end, src) {}

std::string Keyword::to_string() const {
  return "Keyword";
}

Identifier::Identifier(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::Identifier, start, end, src) {}

std::string Identifier::to_string() const {
  return "Identifier";
}

Constant::Constant(
    usize start, usize end, const std::shared_ptr<std::string> &src,
    ConstantType type
):
    Token(TokenType::Constant, start, end, src),
    type_(type) {}

std::string Constant::to_string() const {
  switch (type_) {
    case ConstantType::Integer:
      return "Integer Constant";
    case ConstantType::Float:
      return "Float Constant";
    case ConstantType::Character:
      return "Character Constant";
  }
}

StringLiteral::StringLiteral(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::StringLiteral, start, end, src) {}

std::string StringLiteral::to_string() const {
  return "String Literal";
}

Punctuator::Punctuator(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::Punctuator, start, end, src) {}

std::string Punctuator::to_string() const {
  return "Punctuator";
}

Comment::Comment(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::Comment, start, end, src) {}

std::string Comment::to_string() const {
  return "Comment";
}

Whitespace::Whitespace(
    usize start, usize end, const std::shared_ptr<std::string> &src
):
    Token(TokenType::Whitespace, start, end, src) {}

std::string Whitespace::to_string() const {
  return "Whitespace";
}

LexError::LexError(
    usize start, usize end, const std::shared_ptr<std::string> &src,
    LexErrorType type
):
    Token(TokenType::LexError, start, end, src),
    type_(type) {}

std::string LexError::to_string() const {
  return "LexError";
}

} // namespace clap
