#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace argparse
{

  /**
   * @brief Represents a single command-line option.
   */
  struct Option
  {
    std::string name;
    std::string description;
    bool required = false;
    bool takes_value = false;
    std::string default_value;
  };

  /**
   * @brief Minimal argument parser.
   *
   * Supports:
   * - Flags: --verbose
   * - Options with values: --port 8080
   */
  class Parser
  {
  public:
    Parser(std::string program_name)
        : program_name_(std::move(program_name))
    {
    }

    /**
     * @brief Add a flag (boolean switch).
     */
    void add_flag(const std::string &name,
                  const std::string &description = "")
    {
      options_[name] = Option{name, description, false, false, ""};
    }

    /**
     * @brief Add an option that requires a value.
     */
    void add_option(const std::string &name,
                    const std::string &description = "",
                    bool required = false,
                    const std::string &default_value = "")
    {
      options_[name] = Option{name, description, required, true, default_value};
    }

    /**
     * @brief Parse argc/argv.
     */
    void parse(int argc, char **argv)
    {
      for (int i = 1; i < argc; ++i)
      {
        std::string arg = argv[i];

        if (arg.rfind("--", 0) == 0)
        {
          arg = arg.substr(2);

          auto it = options_.find(arg);
          if (it == options_.end())
            throw std::runtime_error("Unknown option: --" + arg);

          if (it->second.takes_value)
          {
            if (i + 1 >= argc)
              throw std::runtime_error("Missing value for --" + arg);

            values_[arg] = argv[++i];
          }
          else
          {
            values_[arg] = "true";
          }
        }
        else
        {
          positional_.push_back(arg);
        }
      }

      validate_required();
    }

    /**
     * @brief Returns true if a flag was set.
     */
    bool has(const std::string &name) const
    {
      return values_.count(name) > 0;
    }

    /**
     * @brief Get string value.
     */
    std::string get(const std::string &name) const
    {
      auto it = values_.find(name);
      if (it != values_.end())
        return it->second;

      auto opt = options_.find(name);
      if (opt != options_.end())
        return opt->second.default_value;

      throw std::runtime_error("Option not found: " + name);
    }

    /**
     * @brief Get positional arguments.
     */
    const std::vector<std::string> &positional() const
    {
      return positional_;
    }

    /**
     * @brief Print help.
     */
    void print_help(std::ostream &os = std::cout) const
    {
      os << "Usage: " << program_name_ << " [options]\n\n";
      os << "Options:\n";

      for (const auto &pair : options_)
      {
        const auto &opt = pair.second;
        os << "  --" << opt.name;

        if (opt.takes_value)
          os << " <value>";

        os << "\n      " << opt.description;

        if (!opt.default_value.empty())
          os << " (default: " << opt.default_value << ")";

        if (opt.required)
          os << " [required]";

        os << "\n\n";
      }
    }

  private:
    void validate_required() const
    {
      for (const auto &pair : options_)
      {
        const auto &opt = pair.second;

        if (opt.required)
        {
          if (values_.count(opt.name) == 0 &&
              opt.default_value.empty())
          {
            throw std::runtime_error("Missing required option: --" + opt.name);
          }
        }
      }
    }

  private:
    std::string program_name_;
    std::unordered_map<std::string, Option> options_;
    std::unordered_map<std::string, std::string> values_;
    std::vector<std::string> positional_;
  };

} // namespace argparse
