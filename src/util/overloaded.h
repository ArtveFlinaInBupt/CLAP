#pragma once

#ifndef CLAP_UTIL_OVERLOADED_H
#  define CLAP_UTIL_OVERLOADED_H

namespace clap {

template<class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace clap

#endif // CLAP_UTIL_OVERLOADED_H
