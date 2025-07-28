#include "gpu_utils.hpp"
#include "yaml-cpp/yaml.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<GPUInfo> get_gpu_info() {
  vector<GPUInfo> gpus;
  const char *cmd = "nvidia-smi --query-gpu=index,memory.free,memory.total "
                    "--format=csv,noheader,nounits";
  FILE *pipe = popen(cmd, "r");

  if (!pipe) {
    cerr << "Failed to run nvidia-smi." << endl;
    return gpus;
  }

  char buffer[128];

  while (fgets(buffer, sizeof(buffer), pipe)) {
    istringstream iss(buffer);
    string idx_str, free_mem_str, total_mem_str;

    if (getline(iss, idx_str, ',') && getline(iss, free_mem_str, ',') &&
        getline(iss, total_mem_str)) {
      GPUInfo gpu;
      gpu.index = stoi(idx_str);
      gpu.memory_free_mb = stoi(free_mem_str);
      gpu.memory_total_mb = stoi(total_mem_str);
      gpus.push_back(gpu);
    }
  }
  pclose(pipe);

  return gpus;
}

void inject_yaml(const string &yaml_path, const string &yaml_key,
                 const string &cuda_index) {
  YAML::Node config;
  try {
    config = YAML::LoadFile(yaml_path);
  } catch (YAML::BadFile &e) {
    cout << "Error loading YAML file: " << e.what() << endl;
    return;
  }

  if (!config[yaml_key]) {
    cerr << "Key " << yaml_key << " not found in YAML file." << endl;
    return;
  }

  config[yaml_key] = cuda_index;
  ofstream fout(yaml_path);
  fout << config;
  fout.close();
}

void list_idle_gpus(int min_idle_gb, bool sort_by_memory, bool to_json) {
  int min_idle_mb = min_idle_gb * 1024;
  vector<GPUInfo> gpus = get_gpu_info();
  vector<GPUInfo> filtered;

  for (vector<GPUInfo>::const_iterator it = gpus.begin(); it != gpus.end();
       ++it) {
    const GPUInfo &gpu = *it;
    if (gpu.memory_free_mb >= min_idle_mb) {
      filtered.push_back(gpu);
    }
  }

  if (sort_by_memory) {
    sort(filtered.begin(), filtered.end(),
         [](const GPUInfo &a, const GPUInfo &b) {
           return a.memory_free_mb > b.memory_free_mb;
         });
  }

  if (to_json) {
    cout << "[";
    for (size_t i = 0; i < filtered.size(); ++i) {
      cout << "{\"index\": " << filtered[i].index
           << ", \"memory_free_mb\": " << filtered[i].memory_free_mb << "}";
      if (i < filtered.size() - 1) {
        cout << ",";
      }
    }
    cout << "]" << endl;
  } else {
    for (vector<GPUInfo>::const_iterator it = filtered.begin();
         it != filtered.end(); ++it) {
      const GPUInfo &gpu = *it;
      cout << "cuda:" << gpu.index << " " << gpu.memory_free_mb << " MB free"
           << endl;
    }
  }
}

void list_busy_gpus(bool sort_by_memory, bool to_json) {
  vector<GPUInfo> gpus = get_gpu_info();
  vector<GPUInfo> busy_gpus;

  for (vector<GPUInfo>::const_iterator it = gpus.begin(); it != gpus.end();
       ++it) {
    const GPUInfo &gpu = *it;
    if (gpu.is_busy()) {
      busy_gpus.push_back(gpu);
    }
  }

  if (busy_gpus.empty()) {
    cout << "No busy GPUs found." << endl;
    return;
  }

  if (sort_by_memory) {
    sort(busy_gpus.begin(), busy_gpus.end(),
         [](const GPUInfo &a, const GPUInfo &b) {
           return a.memory_free_mb < b.memory_free_mb;
         });
  }

  if (to_json) {
    cout << "[";
    for (size_t i = 0; i < busy_gpus.size(); ++i) {
      cout << "{\"index\": " << busy_gpus[i].index
           << ", \"memory_free_mb\": " << busy_gpus[i].memory_free_mb << "}";
      if (i < busy_gpus.size() - 1) {
        cout << ",";
      }
    }
    cout << "]" << endl;
  } else {
    for (vector<GPUInfo>::const_iterator it = busy_gpus.begin();
         it != busy_gpus.end(); ++it) {
      const GPUInfo &gpu = *it;
      cout << "cuda:" << gpu.index << " " << gpu.memory_free_mb << " MB free"
           << endl;
    }
  }
}

void select_idle_gpu(int min_idle_gb, bool export_env, bool yaml_inject,
                     optional<string> yaml_path, optional<string> yaml_key) {
  int min_idle_mb = min_idle_gb * 1024;
  vector<GPUInfo> gpus = get_gpu_info();
  vector<int> eligible;

  for (vector<GPUInfo>::const_iterator it = gpus.begin(); it != gpus.end();
       ++it) {
    const GPUInfo &gpu = *it;
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
    setenv("CUDA_VISIBLE_DEVICES", to_string(chosen).c_str(), 1);
  } else if (yaml_inject) {
    inject_yaml(yaml_path.value(), yaml_key.value(),
                "cuda:" + to_string(chosen));
  } else {
    cout << "cuda:" << chosen << endl;
  }
}

void rob_gpu(const string &cuda_index) {
  // TODO: new feature
  cout << " " << endl;
}
