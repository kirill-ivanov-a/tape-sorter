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
#include <random>

#include <gtest/gtest.h>
#include <tape_sorter/sort/tape_sorter.h>

namespace ts = tape_sorter;
namespace fs = std::filesystem;

std::vector<int> GenerateRandomVector(size_t size, int min_value = -1000,
                                      int max_value = 1000) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> distribution(min_value, max_value);

  std::vector<int> random_integers;
  random_integers.reserve(size);
  for (auto i = 0ul; i != size; ++i) {
    random_integers.push_back(distribution(generator));
  }

  return random_integers;
}

class SortData : public ::testing::Test {
  static constexpr const auto kTempTapeFilename = "test_tape";

 protected:
  void SetUp() override {
    input_tape_ = std::make_unique<ts::FileTape>(GetInputTempTapePath());
    output_tape_ = std::make_unique<ts::FileTape>(GetOutputTempTapePath());
  }

  void TearDown() override {
    fs::remove(GetInputTempTapePath());
    fs::remove(GetOutputTempTapePath());
  }

  fs::path GetInputTempTapePath() const {
    return fs::current_path() / kTempTapeFilename;
  }

  fs::path GetOutputTempTapePath() const {
    return fs::current_path() / kTempTapeFilename;
  }

  ts::FileTape& GetInputTape() { return *input_tape_; }

  ts::FileTape& GetOutputTape() { return *output_tape_; }

  std::vector<int> ReadNumbersFromOutputTape() {
    std::ifstream file(GetOutputTempTapePath());
    std::vector<int> content;

    int value;
    while (!file.read(reinterpret_cast<char*>(&value), sizeof(value)).eof()) {
      content.push_back(value);
    }
    return content;
  }

  void WriteNumbersToInputTape(const std::vector<int>& content) {
    std::ofstream file(GetInputTempTapePath());
    for (auto value : content) {
      file.write(reinterpret_cast<char*>(&value), sizeof(int));
    }
  }

 private:
  std::unique_ptr<ts::FileTape> input_tape_;
  std::unique_ptr<ts::FileTape> output_tape_;
};

TEST_F(SortData, Decreasing) {
  constexpr const auto kNumbers = 50;
  constexpr const auto kBufferSize = 10;
  std::vector<int> expected_numbers(kNumbers);
  std::iota(expected_numbers.begin(), expected_numbers.end(), 0);
  std::vector<int> reversed_numbers(expected_numbers.rbegin(),
                                    expected_numbers.rend());
  WriteNumbersToInputTape(reversed_numbers);
  auto& input_tape = GetInputTape();
  auto& output_tape = GetOutputTape();

  ts::TapeSorter(kBufferSize).Sort(input_tape, output_tape);
  ASSERT_EQ(ReadNumbersFromOutputTape(), expected_numbers);
}

class SortDataLengthParametrized : public SortData,
                                   public testing::WithParamInterface<int> {};

TEST_P(SortDataLengthParametrized, RandomValues) {
  auto numbers_size = GetParam();
  constexpr const auto kBufferSize = 50;
  //  constexpr const auto kBufferSize = 10;
  std::vector<int> expected_numbers = GenerateRandomVector(numbers_size);
  WriteNumbersToInputTape(expected_numbers);
  auto& input_tape = GetInputTape();
  auto& output_tape = GetOutputTape();

  ts::TapeSorter(kBufferSize).Sort(input_tape, output_tape);
  std::sort(expected_numbers.begin(), expected_numbers.end());
  ASSERT_EQ(ReadNumbersFromOutputTape(), expected_numbers);
}

INSTANTIATE_TEST_SUITE_P(Sort, SortDataLengthParametrized,
                         testing::Values(1, 10, 1000, 1000000));

class SortDataBufferParametrized : public SortData,
                                   public testing::WithParamInterface<int> {};

TEST_P(SortDataBufferParametrized, RandomValues) {
  constexpr const auto kNumbersSize = 10000;
  const auto buffer_size = GetParam();
  //  constexpr const auto kBufferSize = 10;
  std::vector<int> expected_numbers = GenerateRandomVector(kNumbersSize);
  WriteNumbersToInputTape(expected_numbers);
  auto& input_tape = GetInputTape();
  auto& output_tape = GetOutputTape();

  ts::TapeSorter(buffer_size).Sort(input_tape, output_tape);
  std::sort(expected_numbers.begin(), expected_numbers.end());
  ASSERT_EQ(ReadNumbersFromOutputTape(), expected_numbers);
  ASSERT_EQ(ReadNumbersFromOutputTape(), expected_numbers);
}

INSTANTIATE_TEST_SUITE_P(Sort, SortDataBufferParametrized,
                         testing::Values(1, 10, 1000, 1000000));
