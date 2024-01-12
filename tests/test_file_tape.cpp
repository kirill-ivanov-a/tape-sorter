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

#include <filesystem>
#include <numeric>

#include <gtest/gtest.h>
#include <tape_sorter/file_tape.h>

namespace ts = tape_sorter;
namespace fs = std::filesystem;

class TestTape : public ::testing::Test {
  static constexpr const auto kTempTapeFilename = "test_tape";

 protected:
  void SetUp() override {
    tape_ = std::make_unique<ts::FileTape>(GetTempTapePath());
  }

  void TearDown() override { fs::remove(GetTempTapePath()); }

  fs::path GetTempTapePath() const {
    return fs::current_path() / kTempTapeFilename;
  }

  ts::FileTape& GetTape() { return *tape_; }

  std::vector<int> ReadNumbers() {
    std::ifstream file(GetTempTapePath());
    std::vector<int> content;

    int value;
    while (!file.read(reinterpret_cast<char*>(&value), sizeof(value)).eof()) {
      content.push_back(value);
    }
    return content;
  }

  void WriteNumbers(const std::vector<int>& content) {
    std::ofstream file(GetTempTapePath());
    for (auto value : content) {
      file.write(reinterpret_cast<char*>(&value), sizeof(int));
    }
  }

 private:
  std::unique_ptr<ts::FileTape> tape_;
};

TEST_F(TestTape, Write) {
  auto& tape = GetTape();
  auto value = 123;
  tape.Write(value);
  ASSERT_EQ(value, tape.Read().value());
}

TEST_F(TestTape, MoveForward) {
  constexpr const auto kWritesNumber = 50;
  std::vector<int> expected_numbers(kWritesNumber);
  std::iota(expected_numbers.begin(), expected_numbers.end(), 0);
  WriteNumbers(expected_numbers);

  std::vector<int> actual_numbers;
  actual_numbers.reserve(kWritesNumber);
  auto& tape = GetTape();

  while (tape.Read()) {
    actual_numbers.push_back(tape.Read().value());
    tape.MoveForward();
  }

  ASSERT_EQ(actual_numbers, expected_numbers);
}

TEST_F(TestTape, Rewind) {
  constexpr const auto kWritesNumber = 50;
  std::vector<int> expected_numbers(kWritesNumber);
  std::iota(expected_numbers.begin(), expected_numbers.end(), 0);
  auto& tape = GetTape();
  for (auto i = 0; i != kWritesNumber; ++i) {
    tape.Write(i);
    tape.MoveForward();
  }
  tape.Rewind();
  std::vector<int> actual_numbers;
  actual_numbers.reserve(kWritesNumber);
  while (tape.Read()) {
    actual_numbers.push_back(tape.Read().value());
    tape.MoveForward();
  }
  ASSERT_EQ(actual_numbers, expected_numbers);
}

TEST_F(TestTape, MultipleWrite) {
  constexpr const auto kWritesNumber = 50;
  auto& tape = GetTape();
  std::vector<int> expected;
  expected.reserve(kWritesNumber);
  for (auto i = 0; i != kWritesNumber; ++i) {
    tape.Write(i);
    expected.push_back(i);
    tape.MoveForward();
  }
  ASSERT_EQ(expected, ReadNumbers());
}

TEST_F(TestTape, MoveOverEnd) {
  auto& tape = GetTape();
  ASSERT_EQ(tape.MoveForward(), false);
}

TEST_F(TestTape, MoveBackward) {
  constexpr const auto kWritesNumber = 50;
  std::vector<int> expected_numbers(kWritesNumber);
  std::iota(expected_numbers.begin(), expected_numbers.end(), 0);
  WriteNumbers(expected_numbers);

  std::vector<int> actual_numbers;
  actual_numbers.reserve(kWritesNumber);
  auto& tape = GetTape();

  while (tape.Read()) {
    tape.MoveForward();
  }
  tape.MoveBackward();
  while (tape.Read()) {
    actual_numbers.push_back(tape.Read().value());
    tape.MoveBackward();
  }
  std::reverse(expected_numbers.begin(), expected_numbers.end());

  ASSERT_EQ(actual_numbers, expected_numbers);
}

TEST_F(TestTape, MoveBeyondBeforeBegin) {
  auto& tape = GetTape();
  tape.MoveBackward();  // move to before_begin
  ASSERT_EQ(tape.MoveBackward(), false);
}

TEST_F(TestTape, WriteBeforeBegin) {
  auto& tape = GetTape();
  tape.MoveBackward();  // move to before_begin
  ASSERT_THROW(tape.Write(1), std::out_of_range);
}
