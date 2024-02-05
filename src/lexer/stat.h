#pragma once

#ifndef CLAP_LEXER_STAT_H
#  define CLAP_LEXER_STAT_H

#  include "lexer/file_reader.h"
#  include "lexer/token.h"
#  include "util/all.h"

namespace clap {

class Statistic {
  usize line_cnt_{};
  usize char_cnt_{};
  usize token_cnt_{};
  usize keyword_cnt_{};
  usize identifier_cnt_{};
  usize integer_cnt_{};
  usize float_cnt_{};
  usize character_cnt_{};
  usize string_cnt_{};
  usize punctuator_cnt_{};
  usize comment_cnt_{};
  usize preprocessor_command_cnt_{};
  usize error_cnt_{};

public:
  Statistic() = default;

  void update_by_file(const FileReader &file);

  void update_by_token(TokenBase token);

  [[nodiscard]] std::string to_string() const;
};

} // namespace clap

#endif // CLAP_LEXER_STAT_H
