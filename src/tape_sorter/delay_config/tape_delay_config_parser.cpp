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

#include "tape_sorter/delay_config/tape_delay_config_parser.h"

namespace tape_sorter {

namespace {

static std::string Trim(const std::string& str,
                        std::string trimming_characters = " ") {
  size_t start = str.find_first_not_of(trimming_characters);
  size_t end = str.find_last_not_of(trimming_characters);

  if (start == std::string::npos || end == std::string::npos) {
    return "";
  }

  return str.substr(start, end - start + 1);
}

std::pair<std::string, std::string> ParseKeyValuePair(const std::string& input,
                                                      char equal_sign = '=') {
  std::istringstream iss(input);
  std::string key, value;
  if (std::getline(iss, key, equal_sign) && std::getline(iss, value)) {
    // Check if the key and the value are not empty
    key = Trim(key);
    if (key.empty()) {
      std::stringstream msg_stream;
      msg_stream << "Empty key: " << input << '\n';
      throw std::invalid_argument{msg_stream.str()};
    }
    value = Trim(value);
    if (value.empty()) {
      std::stringstream msg_stream;
      msg_stream << "Empty value: " << input << '\n';
      throw std::invalid_argument{msg_stream.str()};
    }
  } else {
    std::stringstream msg_stream;
    msg_stream << "Invalid key-value pair: " << input << " ."
               << "Expected: <key>=<value>.\n";
    throw std::invalid_argument{msg_stream.str()};
  }

  return std::make_pair(key, value);
}

}  // namespace

TapeDelayConfig TapeDelayConfigParser::Parse(const fs::path& config_path) {
  if (!fs::exists(config_path)) {
    throw fs::filesystem_error("File does not exist.", config_path,
                               std::error_code());
  }
  std::ifstream file(config_path);
  TapeDelayConfig config;
  if (file.is_open()) {
    std::unordered_map<std::string, std::chrono::milliseconds> data;
    std::string line;

    while (std::getline(file, line)) {
      auto [key, str_value] = ParseKeyValuePair(line);
      auto value = std::stoll(str_value);
      data[key] = std::chrono::milliseconds(value);
    }

    config.move_delay = data.at(kMoveDelayKey);
    config.read_delay = data.at(kReadDelayKey);
    config.write_delay = data.at(kWriteDelayKey);
    config.rewind_delay = data.at(kRewindDelayKey);
  } else {
    std::stringstream msg_stream;
    msg_stream << "Failed to open file: " << config_path << '\n';
    throw std::ifstream::failure(msg_stream.str());
  }

  return config;
}

}  // namespace tape_sorter
