#include "lexer/file_reader.h"

#include <algorithm>
#include <fstream>

namespace clap {

FileReader::FileReader(const std::string &path): path_{path} {
  std::ifstream fin(path);
  if (!fin.is_open())
    throw std::runtime_error("Failed to open file: " + path);

#ifdef CLAP_LOG
  log(LogLevel::Info, std::format("Reading from file {}.", path));
#endif // CLAP_LOG

  content = std::make_shared<std::string>();
  fin.seekg(0, std::ios::end);
  content->reserve(fin.tellg());
  fin.seekg(0, std::ios::beg);
  content->assign(
      std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()
  );

  line_starts.push_back(0);
  for (decltype(content->size()) i = 0; i < content->size(); i++)
    if (content->at(i) == '\n')
      line_starts.push_back(i + 1);
  if (line_starts.back() >= content->size())
    line_starts.pop_back();
}

usize FileReader::lookup_line(usize pos) const {
  return std::distance(
      line_starts.begin(),
      std::prev(std::upper_bound(line_starts.begin(), line_starts.end(), pos))
  );
}

std::pair<usize, usize> FileReader::lookup_line_and_column(usize pos) const {
  auto line = lookup_line(pos);
  auto column = pos - line_starts[line];
  return {line, column};
}

std::string FileReader::get_line(usize line_number) const {
  if (line_number >= line_starts.size())
    throw std::runtime_error("Line number out of range");
  auto start = line_starts[line_number];
  auto end = line_starts[line_number + 1];
  return content->substr(start, end - start);
}

usize FileReader::len() const {
  return content->size();
}

usize FileReader::max_line_len() const {
  usize max_len = 0;
  for (usize i = 0; i < line_starts.size() - 1; i++) {
    auto len = line_starts[i + 1] - line_starts[i];
    if (len > max_len)
      max_len = len;
  }
  return max_len;
}

usize FileReader::line_count() const {
  return line_starts.size();
}

} // namespace clap
