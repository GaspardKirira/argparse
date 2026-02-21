#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char **argv)
{
  argparse::Parser p("copy");

  p.add_flag("verbose", "Verbose output");

  try
  {
    p.parse(argc, argv);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n";
    p.print_help();
    return 1;
  }

  const auto &pos = p.positional();
  if (pos.size() < 2)
  {
    std::cerr << "Expected: copy <src> <dst>\n";
    return 1;
  }

  std::cout << "src: " << pos[0] << "\n";
  std::cout << "dst: " << pos[1] << "\n";

  if (p.has("verbose"))
    std::cout << "verbose: true\n";

  return 0;
}
