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

#include "tapes_priority_queue.h"

namespace tape_sorter {

namespace {

std::vector<int> ReadBlock(ITape& input_tape, size_t buffer_size) {
  std::vector<int> block;
  block.reserve(buffer_size);

  for (auto i = 0; i != buffer_size && input_tape.Read(); ++i) {
    auto value = input_tape.Read().value();
    block.push_back(value);
    input_tape.MoveForward();
  }
  return block;
}

void WriteBlock(ITape& input_tape, const std::vector<int>& block) {
  for (auto value : block) {
    input_tape.Write(value);
    input_tape.MoveForward();
  }
}

}  // namespace

TapeSorter::TapeSorter(size_t max_buffer_size,
                       std::unique_ptr<ITempTapeCreator> temp_tape_creator)
    : max_buffer_size_(max_buffer_size),
      temp_tape_creator_(std::move(temp_tape_creator)) {}

void TapeSorter::Sort(ITape& input_tape, ITape& output_tape) const {
  TapesPriorityQueue tapes_queue{SplitIntoSortedSubTapes(input_tape)};

  while (!tapes_queue.Empty()) {
    auto min = tapes_queue.Top();
    tapes_queue.Pop();
    output_tape.Write(min);
    output_tape.MoveForward();
  }
}

std::vector<std::unique_ptr<ITape>> TapeSorter::SplitIntoSortedSubTapes(
    ITape& input_tape) const {
  std::vector<std::unique_ptr<ITape>> subtapes;
  while (input_tape.Read()) {
    auto block = ReadBlock(input_tape, max_buffer_size_);
    // sort descending
    std::sort(block.begin(), block.end(),
              [](auto&& lhs, auto&& rhs) { return lhs > rhs; });
    auto temp_tape = temp_tape_creator_->Create();
    WriteBlock(*temp_tape, block);
    // move to last (min) element
    temp_tape->MoveBackward();
    subtapes.push_back(std::move(temp_tape));
  }

  return subtapes;
}

}  // namespace tape_sorter
