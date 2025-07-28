#include "cli_handler.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void CLIHandler::parse_args(int argc, char *argv[]) {
  if (argc < 2 || string(argv[1]) == "--help" || string(argv[1]) == "-h") {
    print_help();
    exit(0);
  }

  option = argv[1];

  if (option == "list_idle") {
    if (argc < 4 || string(argv[2]) != "--idle") {
      cerr << "Usage: wg list_idle --idle <GB> [--json] [--rank]" << endl;
      exit(1);
    }
    min_idle_gb = stoi(argv[3]);
    min_idle_gb_check();
    for (int i = 4; i < argc; ++i) {
      string flag = argv[i];
      if (flag == "--json") {
        json_output = true;
      } else if (flag == "--rank") {
        sort_by_memory = true;
      } else {
        cerr << "Unknown flag: " << flag << endl;
        exit(1);
      }
    }
  } else if (option == "list_busy") {
    for (int i = 2; i < argc; ++i) {
      string flag = argv[i];
      if (flag == "--json") {
        json_output = true;
      } else if (flag == "--rank") {
        sort_by_memory = true;
      } else {
        cerr << "Unknown flag: " << flag << endl;
        exit(1);
      }
    }
  } else if (option == "choose_idle") {
    if (argc < 4 || string(argv[2]) != "--idle") {
      cerr << "Usage: wg choose_idle --idle <GB> [--export|--yaml <path> --key "
              "<key>]"
           << endl;
      exit(1);
    }
    min_idle_gb = stoi(argv[3]);
    min_idle_gb_check();
    for (int i = 4; i < argc; ++i) {
      string flag = argv[i];
      if (flag == "--rob") {
        continue;
      } else if (flag == "--export") {
        export_env = true;
      } else if (flag == "--yaml") {
        if (i + 3 >= argc || string(argv[i + 2]) != "--key") {
          cerr << "Usage: wg choose_idle --idle <GB> --yaml <path> --key <key>"
               << endl;
          exit(1);
        }
        yaml_inject = true;
        yaml_path = argv[i + 1];
        yaml_key = argv[i + 3];
        i += 3;
      } else {
        cerr << "Unknown flag: " << flag << endl;
        exit(1);
      }
    }
  } else {
    cerr << "Unknown command: " << option << endl;
    exit(1);
  }
}

void CLIHandler::min_idle_gb_check() {
  if (min_idle_gb < 0) {
    cerr << "Minimum idle GPU memory must be non-negative." << endl;
    exit(1);
  }
}

void CLIHandler::print_help() {
  cout << R"(Usage: wg <command> [options])

Commands:
    list_idle        List idle GPUs with at least <GB> of free memory.
    list_busy        List busy GPUs with over 50% memory usage.
    choose_idle      Random choose an idle GPU with at least <GB> of free memory.

Options:
    --idle <GB>      Specify the minimum idle GPU memory in GB.
    --json           Output in JSON format.
    --rank           Sort output by memory usage (Descending).
    --export         Export the chosen GPU to environment variables (CUDA_VISIBLE_DEVICES).
    --yaml <path>    Inject the chosen GPU into a YAML file at the specified path and key.
    --key <key>      Specify the key in the YAML file to inject the GPU.
    -h, --help       Show this help message and exit.

Examples:
    wg list_idle --idle 4 --json
    wg list_busy --rank
    wg choose_idle --idle 4 --export
    wg choose_idle --idle 4 --yaml configs/train.yaml --key device
)" << endl;
}
