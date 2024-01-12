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

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include "tape_sorter/delay_config/tape_delay_config.h"
#include "tape_sorter/tape_interface.h"

namespace tape_sorter {

namespace fs = std::filesystem;

class FileTape : public ITape {
 public:
  FileTape(const fs::path& file_path, TapeDelayConfig config = {});

  std::optional<int> Read() override;

  void Write(int value) override;

  bool MoveForward() override;

  bool MoveBackward() override;

  void Rewind() override;

 private:
  void UpdatePosition(std::streampos position);

 private:
  std::fstream tape_stream_;
  std::streampos current_position_{std::fstream::beg};
  TapeDelayConfig delay_config_;
  // boundary marker
  static std::streampos before_begin;
};

}  // namespace tape_sorter
