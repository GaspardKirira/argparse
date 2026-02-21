#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char **argv)
{
  argparse::Parser p("app");

  p.add_flag("verbose", "Enable verbose output");
  p.add_option("port", "Server port", false, "8080");

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

  if (p.has("verbose"))
    std::cout << "verbose: true\n";
  else
    std::cout << "verbose: false\n";

  std::cout << "port: " << p.get("port") << "\n";
  return 0;
}
