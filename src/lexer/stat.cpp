#include "lexer/stat.h"

#include "util/all.h"

#include <variant>

namespace clap {

void Statistic::update_by_file(const FileReader &file) {
  line_cnt_ += file.line_count();
  char_cnt_ += file.content->size();
}

void Statistic::update_by_token(TokenBase token) {
  ++token_cnt_;
  if (std::holds_alternative<Keyword>(token))
    ++keyword_cnt_;
  else if (std::holds_alternative<Identifier>(token))
    ++identifier_cnt_;
  else if (std::holds_alternative<Constant>(token)) {
    switch (std::get<Constant>(token).type_) {
      case ConstantType::Integer:
        ++integer_cnt_;
        break;
      case ConstantType::Float:
        ++float_cnt_;
        break;
      case ConstantType::Character:
        ++character_cnt_;
        break;
    }
  } else if (std::holds_alternative<StringLiteral>(token))
    ++string_cnt_;
  else if (std::holds_alternative<Punctuator>(token))
    ++punctuator_cnt_;
  else if (std::holds_alternative<Comment>(token))
    ++comment_cnt_;
  else if (std::holds_alternative<PreprocessorCommand>(token))
    ++preprocessor_command_cnt_;
  else if (std::holds_alternative<LexError>(token))
    ++error_cnt_;
#ifdef CLAP_LOG
  else
    log(LogLevel::Error, "Unreachable code");
#endif // CLAP_LOG
}

std::string Statistic::to_string() const {
  return std::format(
      "\033[32mStatistic:\033[0m\n"
      "                  Line count: {}\n"
      "             Character count: {}\n"
      "           Total Token count: {}\n"
      "               Keyword count: {}\n"
      "            Identifier count: {}\n"
      "      Integer constant count: {}\n"
      "        Float constant count: {}\n"
      "    Character constant count: {}\n"
      "        String literal count: {}\n"
      "            Punctuator count: {}\n"
      "               Comment count: {}\n"
      "  Preprocessor command count: {}\n"
      "                 Error count: {}\n",
      line_cnt_, char_cnt_, token_cnt_, keyword_cnt_, identifier_cnt_,
      integer_cnt_, float_cnt_, character_cnt_, string_cnt_, punctuator_cnt_,
      comment_cnt_, preprocessor_command_cnt_, error_cnt_
  );
}

} // namespace clap
