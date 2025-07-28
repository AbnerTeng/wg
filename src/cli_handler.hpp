#ifndef CLI_HANDLER_H
#define CLI_HANDLER_H

#include <optional>
#include <string>

class CLIHandler {
public:
  CLIHandler() {
    min_idle_gb = 0;
    option = "";
    export_env = false;
    json_output = false;
    sort_by_memory = false;
    yaml_inject = false;
    rob_gpu = false;
    yaml_path = std::nullopt;
    yaml_key = std::nullopt;
  }
  ~CLIHandler() {}
  void parse_args(int argc, char *argv[]);
  void min_idle_gb_check();
  void print_help();
  int min_idle_gb;
  std::string option;
  bool export_env;
  bool json_output;
  bool sort_by_memory;
  bool yaml_inject;
  bool rob_gpu;
  std::optional<std::string> yaml_path;
  std::optional<std::string> yaml_key;
};

#endif // CLI_HANDLER_H
