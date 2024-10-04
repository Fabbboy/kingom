#ifndef KINGOM_UTIL_RESULT_HH
#define KINGOM_UTIL_RESULT_HH

#include <optional>
#include <stdexcept>
#include <variant>

namespace kingom::util {
struct ValueTag {};
struct ErrorTag {};

template <typename T, typename E>
class Result {
 private:
  std::variant<T, E> value;
  bool hasValue;

 public:
  Result(ValueTag, T v) : value(std::move(v)), hasValue(true) {}
  Result(ErrorTag, E e) : value(std::move(e)), hasValue(false) {}

  Result(const Result& other) = default;
  Result(Result&& other) = default;
  Result& operator=(const Result& other) = default;
  Result& operator=(Result&& other) = default;

  static Result Ok(T v) { return Result(ValueTag{}, std::move(v)); }
  static Result Err(E e) { return Result(ErrorTag{}, std::move(e)); }

  bool is_ok() const { return hasValue; }
  bool is_err() const { return !hasValue; }

  T unwrap() {
    if (hasValue) return std::move(std::get<T>(value));
    throw std::runtime_error("No value present, but error exists");
  }

  E unwrap_err() {
    if (!hasValue) return std::move(std::get<E>(value));
    throw std::runtime_error("No error present");
  }
};
template <typename E>
class Result<void, E> {
 private:
  std::optional<E> error;

 public:
  Result() : error(std::nullopt) {}
  Result(ErrorTag, E e) : error(std::move(e)) {}

  static Result Ok() { return Result(); }
  static Result Err(E e) { return Result(ErrorTag{}, std::move(e)); }

  bool is_ok() const { return !error.has_value(); }
  bool is_err() const { return error.has_value(); }

  void unwrap() {
    if (is_err()) {
      throw std::runtime_error("No value present, but error exists");
    }
  }

  E unwrap_err() {
    if (is_err()) {
      return std::move(*error);
    }
    throw std::runtime_error("No error present");
  }
};
}  // namespace kingom::util

#endif  // KINGOM_UTIL_RESULT_HH