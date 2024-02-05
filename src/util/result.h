#pragma once

#ifndef CLAP_RESULT_H
#  define CLAP_RESULT_H

#  include <stdexcept>
#  include <utility>

namespace clap {

template<typename T, typename E>
class Result {
  union {
    T ok_;
    E err_;
  };

  bool is_ok_;

  Result() {
    is_ok_ = false;
    new (&err_) E();
  }

public:
  Result(const T &ok): is_ok_(true), ok_(ok) {}

  Result(T &&ok): is_ok_(true), ok_(std::move(ok)) {}

  Result(const Result &other) = delete;

  Result(Result &&other) noexcept: is_ok_(other.is_ok_) {
    if (is_ok_)
      new (&ok_) T(std::move(other.ok_));
    else
      new (&err_) E(std::move(other.err_));
  }

  Result &operator=(const Result &other) = delete;

  Result &operator=(Result &&other) noexcept {
    if (is_ok_ && other.is_ok_)
      ok_ = std::move(other.ok_);
    else if (!is_ok_ && !other.is_ok_)
      err_ = std::move(other.err_);
    else {
      if (is_ok_)
        ok_.~T();
      else
        err_.~E();

      is_ok_ = other.is_ok_;
      if (is_ok_)
        new (&ok_) T(std::move(other.ok_));
      else
        new (&err_) E(std::move(other.err_));
    }
    return *this;
  }

  ~Result() {
    if (is_ok_)
      ok_.~T();
    else
      err_.~E();
  }

  static Result<T, E> make_err(const E &err) {
    Result<T, E> result;
    result.is_ok_ = false;
    result.err_ = err;
    return result;
  }

  static Result<T, E> make_err(E &&err) {
    Result<T, E> result;
    result.is_ok_ = false;
    result.err_ = std::move(err);
    return result;
  }

  [[nodiscard]] bool is_ok() const {
    return is_ok_;
  }

  [[nodiscard]] bool is_err() const {
    return !is_ok_;
  }

  [[nodiscard]] const T &unwrap() const {
    if (!is_ok_)
      throw std::runtime_error("Result is not ok");
    return ok_;
  }

  [[nodiscard]] const E &err() const {
    if (is_ok_)
      throw std::runtime_error("Result is not err");
    return err_;
  }

  [[nodiscard]] T &unwrap() {
    if (!is_ok_)
      throw std::runtime_error("Result is not ok");
    return ok_;
  }

  [[nodiscard]] E &err() {
    if (is_ok_)
      throw std::runtime_error("Result is not err");
    return err_;
  }
};

} // namespace clap

#endif // CLAP_RESULT_H
