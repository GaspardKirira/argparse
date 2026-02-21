#include <argparse/argparse.hpp>

#include <iostream>
#include <string>
#include <vector>

static int g_failures = 0;

static void expect_true(bool cond, const std::string &msg)
{
  if (!cond)
  {
    ++g_failures;
    std::cerr << "[FAIL] " << msg << "\n";
  }
}

static void expect_eq_str(const std::string &a, const std::string &b, const std::string &msg)
{
  if (a != b)
  {
    ++g_failures;
    std::cerr << "[FAIL] " << msg << " expected='" << b << "' got='" << a << "'\n";
  }
}

static void expect_eq_size(std::size_t a, std::size_t b, const std::string &msg)
{
  if (a != b)
  {
    ++g_failures;
    std::cerr << "[FAIL] " << msg << " expected=" << b << " got=" << a << "\n";
  }
}

static void test_flag_and_option()
{
  std::vector<std::string> args = {"app", "--verbose", "--port", "8080"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &s : args)
    argv.push_back(&s[0]);

  argparse::Parser p("app");
  p.add_flag("verbose", "Enable verbose output");
  p.add_option("port", "Server port", true);

  p.parse((int)argv.size(), argv.data());

  expect_true(p.has("verbose"), "flag --verbose should be present");
  expect_eq_str(p.get("port"), "8080", "option --port should be parsed");
}

static void test_default_value()
{
  std::vector<std::string> args = {"app"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &s : args)
    argv.push_back(&s[0]);

  argparse::Parser p("app");
  p.add_option("mode", "Run mode", false, "dev");

  p.parse((int)argv.size(), argv.data());

  expect_true(!p.has("mode"), "mode should not be explicitly set");
  expect_eq_str(p.get("mode"), "dev", "default value should be returned");
}

static void test_positional_args()
{
  std::vector<std::string> args = {"app", "input.txt", "output.txt"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &s : args)
    argv.push_back(&s[0]);

  argparse::Parser p("app");
  p.add_flag("verbose", "Enable verbose output");

  p.parse((int)argv.size(), argv.data());

  const auto &pos = p.positional();
  expect_eq_size(pos.size(), 2, "should capture positional args");
  if (pos.size() == 2)
  {
    expect_eq_str(pos[0], "input.txt", "positional[0]");
    expect_eq_str(pos[1], "output.txt", "positional[1]");
  }
}

static void test_unknown_option_errors()
{
  std::vector<std::string> args = {"app", "--nope"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &s : args)
    argv.push_back(&s[0]);

  argparse::Parser p("app");

  bool threw = false;
  try
  {
    p.parse((int)argv.size(), argv.data());
  }
  catch (...)
  {
    threw = true;
  }

  expect_true(threw, "unknown option should throw");
}

static void test_required_option_missing_errors()
{
  std::vector<std::string> args = {"app"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &s : args)
    argv.push_back(&s[0]);

  argparse::Parser p("app");
  p.add_option("port", "Server port", true);

  bool threw = false;
  try
  {
    p.parse((int)argv.size(), argv.data());
  }
  catch (...)
  {
    threw = true;
  }

  expect_true(threw, "missing required option should throw");
}

int main()
{
  test_flag_and_option();
  test_default_value();
  test_positional_args();
  test_unknown_option_errors();
  test_required_option_missing_errors();

  if (g_failures == 0)
  {
    std::cout << "[OK] All tests passed\n";
    return 0;
  }

  std::cerr << "[FAIL] " << g_failures << " test(s) failed\n";
  return 1;
}
