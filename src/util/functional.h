#pragma once

#ifndef CLAP_UTIL_FUNCTIONAL_H
#  define CLAP_UTIL_FUNCTIONAL_H

#  include "util/type.h"

namespace clap {

template<typename T>
usize len(T n) {
  usize count = 0;
  while (n) {
    n /= 10;
    ++count;
  }
  return count;
}

} // namespace clap

#endif // CLAP_UTIL_FUNCTIONAL_H
