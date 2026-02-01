#ifndef UNIQUE_WRAP_HPP
#define UNIQUE_WRAP_HPP

#include <concepts>
#include <cstdint>
#include <utility>

namespace Core {

struct HourTag {};
struct MinuteTag {};
struct SecondTag {};

template <typename T>
concept IsTimeTag = std::same_as<T, HourTag> || std::same_as<T, MinuteTag> ||
                    std::same_as<T, SecondTag>;

template <typename T>
concept ValidTimeStorage =
    std::same_as<T, std::uint8_t> || std::same_as<T, std::uint_fast8_t>;

template <ValidTimeStorage T, IsTimeTag TAG> struct UniqueWrap {
public:
  friend class TimeModel;
  // Default Constructor
  //~ UniqueWrap() = default;

  // Explicit value constructor
  explicit UniqueWrap(T value) : value_(std::move(value)) {}

  // Allow implicit conversion to the underlying type
  operator const T &() const { return value_; }
  operator T &() { return value_; }

  // Getters
  T *operator->() { return &value_; }
  const T *operator->() const { return &value_; }

private:
  T value_;
};
} // namespace Core

#endif
