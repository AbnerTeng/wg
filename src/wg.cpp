#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct GPUInfo {
  int index;
  int memory_free_mb;
};

vector<GPUInfo> get_gpu_info() {
  vector<GPUInfo> gpus;
  const char *cmd =
      "nvidia-smi --query-gpu=index,memory.free --format=csv,noheader,nounits";
  FILE *pipe = popen(cmd, "r");

  if (!pipe) {
    cerr << "Failed to run nvidia-smi." << endl;
    return gpus;
  }

  char buffer[128];

  while (fgets(buffer, sizeof(buffer), pipe)) {
    istringstream iss(buffer);
    string idx_str, mem_str;

    if (getline(iss, idx_str) && getline(iss, mem_str)) {
      GPUInfo gpu;
      gpu.index = stoi(idx_str);
      gpu.memory_free_mb = stoi(mem_str);
      gpus.push_back(gpu);
    }
  }
  pclose(pipe);

  return gpus;
}

void list_idle_gpus(int min_idle_gb, bool sort_by_memory) {
  int min_idle_mb = min_idle_gb * 1024;
  auto gpus = get_gpu_info();

  vector<GPUInfo> filtered;

  for (const auto &gpu : gpus) {
    if (gpu.memory_free_mb >= min_idle_mb) {
      filtered.push_back(gpu);
    }
  }

  if (sort_by_memory) {
    sort(filtered.begin(), filtered.end(),
         [](const GPUInfo &a, const GPUInfo &b) {
           return a.memory_free_mb > b.memory_free_mb;
         });
  };

  for (const auto &gpu_info : filtered) {
    cout << "cuda:" << gpu_info.index << " " << gpu_info.memory_free_mb
         << " MB free" << endl;
  }
}

void choose_idle_gpu(int min_idle_gb, bool export_env) {
  int min_idle_mb = min_idle_gb * 1024;
  auto gpus = get_gpu_info();
  vector<int> eligible;

  for (const auto &gpu : gpus) {
    if (gpu.memory_free_mb >= min_idle_mb) {
      eligible.push_back(gpu.index);
    }
  }

  if (eligible.empty()) {
    cerr << "No idle GPU found with at least " << min_idle_gb << " GB free."
         << endl;
    exit(1);
  }

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(0, eligible.size() - 1);
  int chosen = eligible[dist(gen)];

  if (export_env) {
    cout << "export CUDA_VISIBLE_DEVICES=" << chosen << endl;
  } else {
    cout << "cuda:" << chosen << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cerr << "Usage: wg list|choose --idle <GB> [--rank|--export]" << endl;
    return 1;
  }
  string command = argv[1];
  string option = argv[2];
  int min_idle_gb = stoi(argv[3]);

  bool use_rank = false;
  bool use_export = false;

  for (int i = 4; i < argc; i++) {
    string flag = argv[i];

    if (flag == "--rank") {
      use_rank = true;
    } else if (flag == "--export") {
      use_export = true;
    } else {
      cerr << "Unknown flag: " << flag << endl;
    }
  }

  if (option != "--idle") {
    cerr << "Unknown option: " << option << endl;
    return 1;
  }
  if (command == "list") {
    list_idle_gpus(min_idle_gb, use_rank);
  } else if (command == "choose") {
    choose_idle_gpu(min_idle_gb, use_export);
  } else {
    cerr << "Unknown command: " << command << endl;
    return 1;
  }
  return 0;
}
