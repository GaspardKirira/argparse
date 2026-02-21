# argparse

Lightweight header-only C++ command-line argument parser.

Designed for modern C++ applications that need simple, explicit, and
dependency-free CLI parsing.

## Installation

### Using Vix Registry

``` bash
vix add gaspardkirira/argparse
```

### Manual download

Download the latest release:

👉 https://github.com/GaspardKirira/argparse/releases

Or clone:

``` bash
git clone https://github.com/GaspardKirira/argparse.git
```

Add the `include/` directory to your project.

## Quick Example

``` cpp
#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char** argv)
{
  argparse::Parser parser("app");

  parser.add_flag("verbose", "Enable verbose output");
  parser.add_option("port", "Server port", true);

  try
  {
    parser.parse(argc, argv);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    parser.print_help();
    return 1;
  }

  if (parser.has("verbose"))
    std::cout << "Verbose enabled\n";

  std::cout << "Port: " << parser.get("port") << "\n";
}
```

Example usage:

``` bash
./app --verbose --port 8080
```

## Features

-   Header-only
-   Zero dependencies
-   Simple and explicit API
-   Supports flags (`--verbose`)
-   Supports options with values (`--port 8080`)
-   Supports default values
-   Supports required options
-   Positional arguments support
-   Built-in help generation
-   C++17 compatible

## Supported Argument Types

### Flags

``` cpp
parser.add_flag("verbose");
```

Usage:
```bash
    --verbose
```
### Options with values

``` cpp
parser.add_option("port", "Server port", true);
```

Usage:
```bash
    --port 8080
```

### Default values

``` cpp
parser.add_option("mode", "Run mode", false, "dev");
```

If not provided, `mode` returns `"dev"`.

### Positional arguments

``` cpp
const auto& pos = parser.positional();
```

## Error Handling

The parser throws `std::runtime_error` when:

-   An unknown option is passed
-   A required option is missing
-   An option expecting a value has no value

Example:

``` cpp
try
{
  parser.parse(argc, argv);
}
catch (const std::exception& e)
{
  std::cerr << e.what() << "\n";
}
```

## Tests

Run tests with:

``` bash
vix build
vix tests
```

## Examples

See the `examples/` directory:

-   basic.cpp
-   required.cpp
-   positional.cpp

## Why use this argparse library?

-   No macros
-   No template metaprogramming complexity
-   No heavy frameworks
-   Small footprint
-   Ideal for tools, CLIs, services, and embedded-friendly builds
-   Easy to extend (short flags, type conversion, key=value support)

## License

MIT License.

