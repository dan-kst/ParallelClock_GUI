#ifndef TIME_MODEL_HPP
#define TIME_MODEL_HPP

#include "UniqueWrap.hpp"
#include <cstdint>
#include <format>
#include <string>

namespace Core {

constexpr std::uint8_t MAX_FULL_HOURS = 24;
constexpr std::uint8_t MAX_HOURS = 12;
constexpr std::uint8_t MAX_MINUTES = 60;
constexpr std::uint8_t MAX_SECONDS = 60;

constexpr std::uint8_t MIN_HOURS = 0;
constexpr std::uint8_t MIN_MINUTES = 0;
constexpr std::uint8_t MIN_SECONDS = 0;

// Type wraps
using Hour = UniqueWrap<std::uint8_t, HourTag>;
using Minute = UniqueWrap<std::uint8_t, MinuteTag>;
using Second = UniqueWrap<std::uint8_t, SecondTag>;

// Represents a specific time of day (HH:MM:SS).
// Enforces invariants: 0 <= hour < 24, 0 <= minute < 60, 0 <= second < 60.
class TimeModel {
public:
  // Parameterized constructor
  TimeModel(Hour hour = Hour{0}, Minute minute = Minute{0},
            Second second = Second{0});

  // Setters with validation.
  void SetHour(std::uint8_t hour);
  void SetMinute(std::uint8_t minute);
  void SetSecond(std::uint8_t second);

  // Getters
  [[nodiscard]] auto GetHour() const -> std::uint8_t;
  [[nodiscard]] auto GetMinute() const -> std::uint8_t;
  [[nodiscard]] auto GetSecond() const -> std::uint8_t;

  // Modifiers
  void IncreaseBySecond();
  void IncreaseByMinute();
  void IncreaseByHour();

  /// Format to "HH:MM:SS" format (24-hour)
  /// @returns A formatted string..
  [[nodiscard]] auto To24Hour() const -> std::string;

  /// Format to "HH:MM:SS AM/PM" format (12-hour)
  /// @returns A formatted string.
  [[nodiscard]] auto To12Hour() const -> std::string;

private:
  Hour hour_;
  Minute minute_;
  Second second_;
};

} // namespace Core

#endif // TIME_MODEL_HPP
