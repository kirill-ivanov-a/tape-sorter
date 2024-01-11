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

#include <memory>
#include <queue>
#include <vector>

#include "tape_sorter/tape_interface.h"

namespace tape_sorter {

template <typename Comparator = std::greater<int>>
class TapesPriorityQueue {
 public:
  TapesPriorityQueue(std::vector<std::unique_ptr<tape_sorter::ITape>> tapes);

  int Top();

  void Pop();

  bool Empty();

 private:
  struct QueueItem;

  struct TapeComparator;

 private:
  // The queue holds pointers to temporary tapes until it is destroyed
  std::vector<std::unique_ptr<tape_sorter::ITape>> tapes_;
  std::priority_queue<QueueItem, std::vector<QueueItem>, TapeComparator>
      tapes_queue_;
};

// IMPLEMENTATION

template <typename Comparator>
struct TapesPriorityQueue<Comparator>::QueueItem {
  QueueItem(const std::unique_ptr<tape_sorter::ITape>& tape)
      : ptr{tape.get()}, min_value{tape->Read().value()} {}
  tape_sorter::ITape* ptr;
  int min_value;
};

template <typename Comparator>
struct TapesPriorityQueue<Comparator>::TapeComparator {
  bool operator()(const QueueItem& lhs, const QueueItem& rhs) {
    return Comparator{}(lhs.min_value, rhs.min_value);
  }
};

template <typename Comparator>
inline TapesPriorityQueue<Comparator>::TapesPriorityQueue(
    std::vector<std::unique_ptr<tape_sorter::ITape>> tapes)
    : tapes_(std::move(tapes)), tapes_queue_{tapes_.begin(), tapes_.end()} {}

template <typename Comparator>
inline bool TapesPriorityQueue<Comparator>::Empty() {
  return tapes_queue_.empty();
}

template <typename Comparator>
inline int TapesPriorityQueue<Comparator>::Top() {
  auto min_value_tape = tapes_queue_.top();
  return min_value_tape.min_value;
}

template <typename Comparator>
inline void TapesPriorityQueue<Comparator>::Pop() {
  auto min_value_tape = tapes_queue_.top();
  tapes_queue_.pop();
  min_value_tape.ptr->MoveBackward();
  auto new_tape_min_value = min_value_tape.ptr->Read();
  if (new_tape_min_value) {
    min_value_tape.min_value = new_tape_min_value.value();
    tapes_queue_.push(min_value_tape);
  }
}

}  // namespace tape_sorter
