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

#include "tape_sorter/file_tape.h"

namespace tape_sorter {

namespace fs = std::filesystem;

FileTape::FileTape(const fs::path& file_path, TapeDelayConfig config)
    : delay_config_(std::move(config)) {
  if (!std::filesystem::exists(file_path)) {
    std::ofstream{file_path};
  }
  tape_stream_ = std::fstream{
      file_path, std::fstream::in | std::fstream::out | std::fstream::binary};
}

std::optional<int> FileTape::Read() {
  std::this_thread::sleep_for(delay_config_.read_delay);
  if (current_position_ == before_begin) {
    return std::nullopt;
  }
  int value;
  tape_stream_.read(reinterpret_cast<char*>(&value), sizeof(value));
  if (tape_stream_.eof()) {
    // Reset state
    tape_stream_.clear();
    return std::nullopt;
  }
  UpdatePosition(current_position_);
  return value;
}

void FileTape::Write(int value) {
  std::this_thread::sleep_for(delay_config_.write_delay);
  if (current_position_ == before_begin) {
    throw std::out_of_range(
        "Writing to the before begin position is prohibited\n");
  }
  tape_stream_.write(reinterpret_cast<char*>(&value), sizeof(value));
  UpdatePosition(current_position_);
}

bool FileTape::MoveForward() {
  std::this_thread::sleep_for(delay_config_.move_delay);
  if (!Read()) {
    return false;
  }
  UpdatePosition(current_position_ + std::streamoff(sizeof(int)));

  return true;
}

bool FileTape::MoveBackward() {
  std::this_thread::sleep_for(delay_config_.move_delay);
  if (current_position_ == before_begin) {
    return false;
  }
  UpdatePosition(current_position_ - std::streamoff(sizeof(int)));

  return true;
}

void FileTape::Rewind() {
  std::this_thread::sleep_for(delay_config_.rewind_delay);
  // Reset state
  tape_stream_.clear();
  UpdatePosition(std::fstream::beg);
}

void FileTape::UpdatePosition(std::streampos position) {
  current_position_ = position;
  tape_stream_.seekg(current_position_);
  tape_stream_.seekp(current_position_);
}

std::streampos FileTape::before_begin{std::fstream::beg -
                                      std::streamoff(sizeof(int))};
}  // namespace tape_sorter
