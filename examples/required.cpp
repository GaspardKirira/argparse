#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char **argv)
{
  argparse::Parser p("server");

  p.add_option("host", "Bind host", true);
  p.add_option("port", "Bind port", true);

  try
  {
    p.parse(argc, argv);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n\n";
    p.print_help();
    return 1;
  }

  std::cout << "host: " << p.get("host") << "\n";
  std::cout << "port: " << p.get("port") << "\n";
  return 0;
}
