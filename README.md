# tape-sorter
[![tests](https://github.com/kirill-ivanov-a/tape-sorter/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/kirill-ivanov-a/tape-sorter/actions/workflows/tests.yml)
[![clang-format](https://github.com/kirill-ivanov-a/tape-sorter/actions/workflows/clang_format.yml/badge.svg?branch=main)](https://github.com/kirill-ivanov-a/tape-sorter/actions/workflows/clang_format.yml)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Application for sorting tapes providing a read, write and move interface.
## File tape
At the moment, a tape has been implemented, represented using the file.
## Sort
Since the tape may not fit completely in memory, an external sorting algorithm is used: several 
temporary tapes are created, which are merged into the output tape
## Build
1. Install `Boost.Program_options` for console parsing:
```shell
sudo apt update
sudo apt install -y libboost-program-options-dev
```
2. Build sources:
```shell
mkdir build \
&& cd build \
&& cmake .. -DCMAKE_BUILD_TYPE=Release && make -j4
```
## Console demo
See `demos/console`
### Delay config
To simulate tape delays, a configuration file in the following format is used:
```shell
move_delay = <NUM>
read_delay = <NUM>
write_delay = <NUM>
rewind_delay = <NUM>
```
`<NUM>` represents an integer expressing the delay of the operation in milliseconds.
### Usage
```shell
Usage: ./console_demo <INPUT_PATH> <OUTPUT_PATH> <DELAY_CONFIG_PATH> [options]
Allowed options:
  --help                Produce help message
  --input-path arg      Path to input file tape
  --output-path arg     Path to output file tape
  --delay-path arg      Path to tape delay config
  --buffer arg (=50)    Max buffer size
```
## Quick Example

```c++
#include <tape_sorter/sort/tape_sorter.h>
#include <tape_sorter/sort/temp_file_tape_creator.h>

int main {
    auto input_tape_path = std::filesystem::path{
        "<INPUT_PATH>"};
    auto output_tape_path = std::filesystem::path{
            "<OUTPUT_PATH>"};
    
    auto input_tape = tape_sorter::FileTape{input_tape_path};
    auto output_tape = tape_sorter::FileTape{output_tape_path};
    
    auto buffer_size = 50;
    tape_sorter::TapeSorter{buffer_size}.Sort(input_tape, output_tape);
    
    return 0;
};
```


