#ifndef GPU_UTILS_H
#define GPU_UTILS_H

#include <optional>
#include <iostream>
#include <string>

using namespace std;

struct GPUInfo {
    int index;
    int memory_free_mb;
    int memory_total_mb;
    bool is_busy() const {
        return memory_free_mb < memory_total_mb / 2;
    }
};

vector<GPUInfo> get_gpu_info();
void list_idle_gpus(int min_idle_gb, bool sort_by_memory = false, bool to_json = false);
void list_busy_gpus(bool sort_by_memory = false, bool to_json = false);
void inject_yaml(const string& yaml_path, const string& yaml_key, const string& cuda_index);
void select_idle_gpu(int min_idle_gb, bool export_env, bool yaml_inject, optional<string> yaml_path, optional<string> yaml_key);

#endif // GPU_UTILS_H