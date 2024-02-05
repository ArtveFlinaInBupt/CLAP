#pragma once

#ifndef CLAP_LEXER_FILE_READER_H
#  define CLAP_LEXER_FILE_READER_H

#  include "util/all.h"

#  include <string>
#  include <vector>

namespace clap {

class FileReader {
  std::string path_{};

public:
  std::shared_ptr<std::string> content{};
  std::vector<usize> line_starts{};

  explicit FileReader(const std::string &path);

  FileReader(const FileReader &rhs) = delete;

  FileReader(FileReader &&rhs) noexcept = default;

  FileReader &operator=(const FileReader &rhs) = delete;

  FileReader &operator=(FileReader &&rhs) noexcept = default;

  ~FileReader() = default;

  [[nodiscard]] usize lookup_line(usize pos) const;

  [[nodiscard]] std::pair<usize, usize> lookup_line_and_column(usize pos) const;

  [[nodiscard]] std::string get_line(usize line_number) const;

  [[nodiscard]] usize len() const;

  [[nodiscard]] usize max_line_len() const;

  [[nodiscard]] usize line_count() const;
};

} // namespace clap

#endif // CLAP_LEXER_FILE_READER_H
