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

#include "tape_sorter/sort/tape_sorter.h"
#include "tape_sorter/sort/temp_file_tape_creator.h"

int main() {
  std::filesystem::path input_tape_path{""};

  std::filesystem::path output_tape_path{""};

  auto input_tape = tape_sorter::FileTape{input_tape_path};
  auto output_tape = tape_sorter::FileTape{output_tape_path};

  size_t buffer_size = 50;
  tape_sorter::TapeSorter{buffer_size}.Sort(input_tape, output_tape);

  return 0;
}
