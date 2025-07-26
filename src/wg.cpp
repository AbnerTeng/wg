#include "cli_handler.hpp"
#include "gpu_utils.hpp"


int main(int argc, char *argv[]) {
  CLIHandler cli;
  cli.parse_args(argc, argv);
  if (cli.option == "list_idle") {
    list_idle_gpus(cli.min_idle_gb, cli.sort_by_memory, cli.json_output);
  } else if (cli.option == "list_busy") {
    list_busy_gpus(cli.sort_by_memory, cli.json_output);
  } else if (cli.option == "choose_idle") {
    if (cli.yaml_inject) {
      if (!cli.yaml_path.has_value() || !cli.yaml_key.has_value()) {
        std::cerr << "Error: --yaml requires both --yaml_path and --yaml_key to be specified." << std::endl;
        return 1;
      }
      select_idle_gpu(
        cli.min_idle_gb,
        cli.export_env,
        cli.yaml_inject,
        cli.yaml_path.value(),
        cli.yaml_key.value()
      );
    }
    select_idle_gpu(cli.min_idle_gb, cli.export_env, cli.yaml_inject, cli.yaml_path.value(), cli.yaml_key.value());
  } else {
    return 1;
  };
  return 0;
}