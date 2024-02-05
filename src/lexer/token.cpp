#include "lexer/token.h"

#include <string>

namespace clap {

using namespace std::literals::string_literals;

Token::Token(
    TokenBase token, usize start, usize end,
    const std::shared_ptr<std::string> &src
):
    src_(src),
    start_(start), end_(end), token_(token) {}

TokenBase Token::inner() const noexcept {
  return token_;
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

std::string Token::get_name() const {
  return std::visit(
      overloaded{
          [](Keyword) {
            return "             Keyword"s;
          },
          [](Identifier) {
            return "          Identifier"s;
          },
          [](Constant constant) {
            switch (constant.type_) {
              case ConstantType::Integer:
                return "    Integer Constant"s;
              case ConstantType::Float:
                return "      Float Constant"s;
              case ConstantType::Character:
                return "  Character Constant"s;
            }
#ifdef CLAP_LOG
            log(LogLevel::Error, "Unreachable code");
#endif                      // CLAP_LOG
            return "EVIL"s; // unreachable
          },
          [](StringLiteral) {
            return "      String Literal"s;
          },
          [](Punctuator) {
            return "          Punctuator"s;
          },
          [](PreprocessorCommand) {
            return "Preprocessor Command"s;
          },
          [](Comment) {
            return "             Comment"s;
          },
          [](Whitespace) {
            return "          Whitespace"s;
          },
          [](LexError lex_error) {
            switch (lex_error.type_) {
              case LexErrorType::UnclosedBlockComment:
                return "Unclosed Block Comment"s;
              case LexErrorType::UnclosedStringLiteral:
                return "Unclosed String Literal"s;
              case LexErrorType::UnclosedCharacterConstant:
                return "Unclosed Character Constant"s;
              case LexErrorType::IllegalCharacter:
                return "Invalid Character"s;
              case LexErrorType::IllegalFloatConstant:
                return "Invalid Float Constant"s;
              case LexErrorType::IllegalIntegerConstant:
                return "Invalid Integer Constant"s;
              case LexErrorType::IllegalPunctuator:
                return "Invalid Punctuator"s;
            }
#ifdef CLAP_LOG
            log(LogLevel::Error, "Unreachable code");
#endif                      // CLAP_LOG
            return "EVIL"s; // unreachable
          },
      },
      token_
  );
}

std::string Token::to_string() const {
  std::string s;
  s.append(get_name())
      .append(" | ")
      .append(src_->substr(start_, end_ - start_));
  return s;
}

Constant::Constant(ConstantType type): type_(type) {}

LexError::LexError(LexErrorType type): type_(type) {}

} // namespace clap
