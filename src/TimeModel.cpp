#include "TimeModel.hpp"

#include <format>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace Core {

TimeModel::TimeModel(Hour hour, Minute minute, Second second)
    : hour_(0), minute_(0), second_(0) {
  SetHour(static_cast<std::uint8_t>(hour));
  SetMinute(static_cast<std::uint8_t>(minute));
  SetSecond(static_cast<std::uint8_t>(second));
}

void TimeModel::SetHour(std::uint8_t hour) {
  hour_ = Hour{static_cast<std::uint8_t>(hour % MAX_FULL_HOURS)};
}

void TimeModel::SetMinute(std::uint8_t minute) {
  minute_ = Minute{static_cast<std::uint8_t>(minute % MAX_MINUTES)};
}

void TimeModel::SetSecond(std::uint8_t second) {
  second_ = Second{static_cast<std::uint8_t>(second % MAX_SECONDS)};
}

[[nodiscard]] auto TimeModel::GetHour() const -> std::uint8_t {
  return static_cast<std::uint8_t>(hour_);
}

[[nodiscard]] auto TimeModel::GetMinute() const -> std::uint8_t {
  return static_cast<std::uint8_t>(minute_);
}

[[nodiscard]] auto TimeModel::GetSecond() const -> std::uint8_t {
  return static_cast<std::uint8_t>(second_);
}

void TimeModel::IncreaseBySecond() {
  auto raw_second = GetSecond() + 1;
  if (raw_second >= MAX_SECONDS) {
    raw_second = MIN_SECONDS;
    IncreaseByMinute();
  }
  second_ = Second{static_cast<std::uint8_t>(raw_second)};
}

void TimeModel::IncreaseByMinute() {
  auto raw_minute = GetMinute() + 1;
  if (raw_minute >= MAX_MINUTES) {
    raw_minute = MIN_MINUTES;
    IncreaseByHour();
  }
  minute_ = Minute{static_cast<std::uint8_t>(raw_minute)};
}

void TimeModel::IncreaseByHour() {
  auto raw_hour = GetHour() + 1;
  if (raw_hour >= MAX_FULL_HOURS) {
    raw_hour = MIN_HOURS;
  }
  hour_ = Hour{static_cast<std::uint8_t>(raw_hour)};
}

auto TimeModel::To24Hour() const -> std::string {
  return std::format("{:02}:{:02}:{:02}", GetHour(), GetMinute(), GetSecond());
}

auto TimeModel::To12Hour() const -> std::string {
  std::uint8_t display_hour = static_cast<std::uint8_t>(hour_) % MAX_HOURS;

  display_hour = display_hour == 0 ? MAX_HOURS : display_hour;
  std::string_view period = (this->hour_ >= MAX_HOURS) ? "PM" : "AM";

  return std::format("{:02}:{:02}:{:02} {}", display_hour, GetMinute(),
                     GetSecond(), period);
}

} // namespace Core
