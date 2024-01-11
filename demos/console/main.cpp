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

#include <boost/program_options.hpp>

#include "tape_sorter/sort/tape_sorter.h"
#include "tape_sorter/sort/temp_file_tape_creator.h"

namespace po = boost::program_options;

void PrintHelpMessage(const std::string &program_name,
                      const po::options_description &optionals) {
  std::cout << "Usage: " << program_name
            << " <INPUT_PATH> <OUTPUT_PATH> <DELAY_CONFIG_PATH> [options]\n";
  std::cout << optionals << '\n';
}

int main(int argc, char *argv[]) {
  constexpr const auto kInputFileTapePath = "input-path";
  constexpr const auto kOutputFileTapePath = "output-path";
  constexpr const auto kDelayConfigPath = "delay-path";
  constexpr const auto kMaxBufferSize = "buffer";

  po::options_description options_description("Allowed options");
  options_description.add_options()("help", "Produce help message")(
      kInputFileTapePath, po::value<std::string>()->required(),
      "Path to input file tape")(kOutputFileTapePath,
                                 po::value<std::string>()->required(),
                                 "Path to output file tape")(
      kDelayConfigPath, po::value<std::string>()->required(),
      "Path to tape delay config")(kMaxBufferSize,
                                   po::value<size_t>()->default_value(50),
                                   "Max buffer size");

  po::variables_map parsed_variables;
  po::store(
      po::command_line_parser(argc, argv).options(options_description).run(),
      parsed_variables);
  if (parsed_variables.count("help") != 0u) {
    PrintHelpMessage(argv[0], options_description);
  } else {
    po::notify(parsed_variables);
    auto input_tape_path = std::filesystem::path{
        parsed_variables[kInputFileTapePath].as<std::string>()};
    auto output_tape_path = std::filesystem::path{
        parsed_variables[kOutputFileTapePath].as<std::string>()};

    auto input_tape = tape_sorter::FileTape{input_tape_path};
    auto output_tape = tape_sorter::FileTape{output_tape_path};

    auto buffer_size = parsed_variables[kMaxBufferSize].as<size_t>();
    tape_sorter::TapeSorter{buffer_size}.Sort(input_tape, output_tape);
  }

  return 0;
}
