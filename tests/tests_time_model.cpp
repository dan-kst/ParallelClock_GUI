#include "TimeModel.hpp"
#include <array>
#include <gtest/gtest.h>

using namespace Core;

namespace Tests {
struct TimeTestParam {
  int inputH{0};
  int inputM{0};
  int inputS{0};
  int expectedH{0};
  int expectedM{0};
  int expectedS{0};
  std::string expected24{};
  std::string expected12{};
  std::string label{};
};

// Test Tick/Increment logic including rollover
class TimeRolloverTest : public ::testing::TestWithParam<TimeTestParam> {};
// Test Formats
class TimeFormattingTest : public ::testing::TestWithParam<TimeTestParam> {};
class TimeStateTest : public ::testing::TestWithParam<TimeTestParam> {};

TEST_P(TimeFormattingTest, CorrectStringConversion) {
  const auto &iter_param = GetParam();

  TimeModel time(Hour{static_cast<uint8_t>(iter_param.inputH)},
                 Minute{static_cast<uint8_t>(iter_param.inputM)},
                 Second{static_cast<uint8_t>(iter_param.inputS)});

  EXPECT_EQ(time.To24Hour(), iter_param.expected24);
  EXPECT_EQ(time.To12Hour(), iter_param.expected12);
}

TEST_P(TimeRolloverTest, IncrementsCorrectly) {
  const auto &iter_param = GetParam();
  TimeModel time(Hour{static_cast<uint8_t>(iter_param.inputH)},
                 Minute{static_cast<uint8_t>(iter_param.inputM)},
                 Second{static_cast<uint8_t>(iter_param.inputS)});

  time.IncreaseBySecond();

  EXPECT_EQ(time.GetHour(), iter_param.expectedH)
      << "Failed on: " << iter_param.label;
  EXPECT_EQ(time.GetMinute(), iter_param.expectedM)
      << "Failed on: " << iter_param.label;
  EXPECT_EQ(time.GetSecond(), iter_param.expectedS)
      << "Failed on: " << iter_param.label;
}

TEST_P(TimeStateTest, CorrectConstructorInput) {
  const auto &iter_param = GetParam();

  // 1. Test Constructor Validation
  TimeModel time_from_ctor(Hour{static_cast<uint8_t>(iter_param.inputH)},
                           Minute{static_cast<uint8_t>(iter_param.inputM)},
                           Second{static_cast<uint8_t>(iter_param.inputS)});

  EXPECT_EQ(time_from_ctor.GetHour(), iter_param.expectedH)
      << "Ctor fail: " << iter_param.label;
  EXPECT_EQ(time_from_ctor.GetMinute(), iter_param.expectedM)
      << "Ctor fail: " << iter_param.label;
  EXPECT_EQ(time_from_ctor.GetSecond(), iter_param.expectedS)
      << "Ctor fail: " << iter_param.label;
}
TEST_P(TimeStateTest, CorrectSetterInput) {
  const auto &iter_param = GetParam();

  TimeModel time_from_setters;
  time_from_setters.SetHour(static_cast<uint8_t>(iter_param.inputH));
  time_from_setters.SetMinute(static_cast<uint8_t>(iter_param.inputM));
  time_from_setters.SetSecond(static_cast<uint8_t>(iter_param.inputS));

  EXPECT_EQ(time_from_setters.GetHour(), iter_param.expectedH)
      << "Setter fail: " << iter_param.label;
  EXPECT_EQ(time_from_setters.GetMinute(), iter_param.expectedM)
      << "Setter fail: " << iter_param.label;
  EXPECT_EQ(time_from_setters.GetSecond(), iter_param.expectedS)
      << "Setter fail: " << iter_param.label;
}

INSTANTIATE_TEST_SUITE_P(
    ClockFormats, TimeFormattingTest,
    ::testing::Values(TimeTestParam{.inputH = 4,
                                    .inputM = 18,
                                    .inputS = 3,
                                    .expected24 = "04:18:03",
                                    .expected12 = "04:18:03 AM"},
                      TimeTestParam{.inputH = 12,
                                    .inputM = 0,
                                    .inputS = 0,
                                    .expected24 = "12:00:00",
                                    .expected12 = "12:00:00 PM"},
                      TimeTestParam{.inputH = 18,
                                    .inputM = 34,
                                    .inputS = 47,
                                    .expected24 = "18:34:47",
                                    .expected12 = "06:34:47 PM"},
                      TimeTestParam{.inputH = 0,
                                    .inputM = 0,
                                    .inputS = 0,
                                    .expected24 = "00:00:00",
                                    .expected12 = "12:00:00 AM"}));

INSTANTIATE_TEST_SUITE_P(
    TickLogic, TimeRolloverTest,
    ::testing::Values(TimeTestParam{.inputH = 0,
                                    .inputM = 0,
                                    .inputS = 58,
                                    .expectedH = 0,
                                    .expectedM = 0,
                                    .expectedS = 59,
                                    .label = "Simple Second Increment"},
                      TimeTestParam{.inputH = 0,
                                    .inputM = 0,
                                    .inputS = 59,
                                    .expectedH = 0,
                                    .expectedM = 1,
                                    .expectedS = 0,
                                    .label = "Minute Rollover"},
                      TimeTestParam{.inputH = 0,
                                    .inputM = 59,
                                    .inputS = 59,
                                    .expectedH = 1,
                                    .expectedM = 0,
                                    .expectedS = 0,
                                    .label = "Hour Rollover"},
                      TimeTestParam{.inputH = 23,
                                    .inputM = 59,
                                    .inputS = 59,
                                    .expectedH = 0,
                                    .expectedM = 0,
                                    .expectedS = 0,
                                    .label = "Day Rollover (Midnight)"}));

INSTANTIATE_TEST_SUITE_P(
    ValidationLogic, TimeStateTest,
    ::testing::Values(TimeTestParam{.inputH = 18,
                                    .inputM = 6,
                                    .inputS = 32,
                                    .expectedH = 18,
                                    .expectedM = 6,
                                    .expectedS = 32,
                                    .label = "Valid time"},
                      TimeTestParam{.inputH = 25,
                                    .inputM = 60,
                                    .inputS = 61,
                                    .expectedH = 1,
                                    .expectedM = 0,
                                    .expectedS = 1,
                                    .label = "Overflow trimming"},
                      TimeTestParam{.inputH = 48,
                                    .inputM = 120,
                                    .inputS = 180,
                                    .expectedH = 0,
                                    .expectedM = 0,
                                    .expectedS = 0,
                                    .label = "Multiple overflow trimming"}));
} // namespace Tests
