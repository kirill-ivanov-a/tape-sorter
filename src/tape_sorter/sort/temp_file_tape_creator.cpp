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

#include "tape_sorter/sort/temp_file_tape_creator.h"

namespace tape_sorter {

namespace {

fs::path CreateTemporaryFilePath() {
  // NOTE: not thread safe
  return fs::path(std::tmpnam(nullptr));
}

}  // namespace

TempFileTapeCreator::TempFileTapeCreator(TapeDelayConfig config)
    : config_(std::move(config)) {}

std::unique_ptr<ITape> TempFileTapeCreator::Create() {
  auto temp_file = CreateTemporaryFilePath();
  return std::make_unique<FileTape>(temp_file, config_);
}

}  // namespace tape_sorter
