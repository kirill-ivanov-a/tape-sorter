//   Copyright (c) 2023, Kirill Ivanov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <gtest/gtest.h>
#include <tape_sorter/delay_config/tape_delay_config_parser.h>

#include <filesystem>
#include <random>

namespace ts = tape_sorter;
namespace fs = std::filesystem;

class TestDelayConfig : public ::testing::Test {
  static constexpr const auto kTempTapeFilename = "test_config";

 protected:
  void TearDown() override { fs::remove(GetTempConfigPath()); }

  fs::path GetTempConfigPath() const {
    return fs::current_path() / kTempTapeFilename;
  }
  void WriteConfig(const std::stringstream& config) {
    std::ofstream file(GetTempConfigPath());
    file << config.str();
  }
};

TEST_F(TestDelayConfig, Correct) {
  std::stringstream config_stream;
  auto move_delay = 1;
  auto read_delay = 2;
  auto write_delay = 3;
  auto rewind_delay = 4;

  config_stream << "move_delay = " << move_delay << '\n';
  config_stream << "read_delay = " << read_delay << '\n';
  config_stream << "write_delay = " << write_delay << '\n';
  config_stream << "rewind_delay = " << rewind_delay << '\n';
  WriteConfig(config_stream);
  auto config = ts::TapeDelayConfigParser::Parse(GetTempConfigPath());

  ASSERT_EQ(config.move_delay.count(), move_delay);
  ASSERT_EQ(config.read_delay.count(), read_delay);
  ASSERT_EQ(config.write_delay.count(), write_delay);
  ASSERT_EQ(config.rewind_delay.count(), rewind_delay);
}

TEST_F(TestDelayConfig, EmptyKey) {
  std::stringstream config_stream;
  config_stream << "move_delay =";
  WriteConfig(config_stream);
  ASSERT_THROW(ts::TapeDelayConfigParser::Parse(GetTempConfigPath()),
               std::invalid_argument);
}

TEST_F(TestDelayConfig, EmptyValue) {
  std::stringstream config_stream;
  config_stream << " = 1";
  WriteConfig(config_stream);
  ASSERT_THROW(ts::TapeDelayConfigParser::Parse(GetTempConfigPath()),
               std::invalid_argument);
}

TEST_F(TestDelayConfig, FailedCastToInt) {
  std::stringstream config_stream;
  config_stream << "f = a";
  WriteConfig(config_stream);
  ASSERT_THROW(ts::TapeDelayConfigParser::Parse(GetTempConfigPath()),
               std::invalid_argument);
}

TEST_F(TestDelayConfig, NotEnoughParameters) {
  std::stringstream config_stream;
  config_stream << "f = 1";
  WriteConfig(config_stream);
  ASSERT_THROW(ts::TapeDelayConfigParser::Parse(GetTempConfigPath()),
               std::out_of_range);
}
