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

#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

#include "tape_sorter/sort/temp_file_tape_creator.h"
#include "tape_sorter/tape_interface.h"

namespace tape_sorter {

class TapeSorter final {
 public:
  TapeSorter(size_t max_buffer_size,
             std::unique_ptr<ITempTapeCreator> temp_tape_creator =
                 std::make_unique<TempFileTapeCreator>());

  void Sort(ITape& input_tape, ITape& output_tape) const;

 private:
  std::vector<std::unique_ptr<ITape>> SplitIntoSortedSubTapes(
      ITape& input_tape) const;

 private:
  size_t max_buffer_size_;
  std::unique_ptr<ITempTapeCreator> temp_tape_creator_;
};

}  // namespace tape_sorter
