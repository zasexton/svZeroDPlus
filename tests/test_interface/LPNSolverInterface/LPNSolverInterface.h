
#ifdef _WIN32
  #include <windows.h>
  using dl_handle_t = HMODULE;
  inline dl_handle_t dlopen(const char *lib, int /*flags*/) {
    return LoadLibraryA(lib);
  }
  inline void *dlsym(dl_handle_t h, const char *sym) {
    FARPROC p = GetProcAddress(h, sym);
    return reinterpret_cast<void*>(p);
  }
  inline int dlclose(dl_handle_t h) {
    return FreeLibrary(h) ? 0 : -1;
  }
  inline const char *dlerror() { return ""; }
#else
  #include <dlfcn.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdexcept>
  
#ifndef LPNSolverInterface_h
#define LPNSolverInterface_h

//--------------------
// LPNSolverInterface
//--------------------
//
class LPNSolverInterface
{
  public:
    LPNSolverInterface();
    ~LPNSolverInterface();

    void load_library(const std::string& interface_lib);
    void initialize(std::string file_name);
    void increment_time(const double time, std::vector<double>& solution);
    void run_simulation(const double time, std::vector<double>& output_times, std::vector<double>& output_solutions, int& error_code);
    void update_block_params(std::string block_name, std::vector<double>& new_params);
    void read_block_params(std::string block_name, std::vector<double>& block_params);
    void get_block_node_IDs(std::string block_name, std::vector<int>& IDs);
    void update_state(std::vector<double> state_y, std::vector<double> state_ydot);
    void return_y(std::vector<double>& y);
    void return_ydot(std::vector<double>& ydot);
    void set_external_step_size(double step_size);

    // Interface functions.
    std::string lpn_initialize_name_;
    void (*lpn_initialize_)(std::string, int&, int&, int&, int&, std::vector<std::string>&, std::vector<std::string>&);

    std::string lpn_increment_time_name_;
    void (*lpn_increment_time_)(const int, const double, std::vector<double>& solution);

    std::string lpn_run_simulation_name_;
    void (*lpn_run_simulation_)(const int, const double, std::vector<double>& output_times, std::vector<double>& output_solutions, int& error_code);
    
    std::string lpn_update_block_params_name_;
    void (*lpn_update_block_params_)(const int, std::string, std::vector<double>& new_params);
    
    std::string lpn_read_block_params_name_;
    void (*lpn_read_block_params_)(const int, std::string, std::vector<double>& block_params);
    
    std::string lpn_get_block_node_IDs_name_;
    void (*lpn_get_block_node_IDs_)(const int, std::string, std::vector<int>& block_params);
    
    std::string lpn_update_state_name_;
    void (*lpn_update_state_)(const int, std::vector<double>, std::vector<double>);
    
    std::string lpn_return_y_name_;
    void (*lpn_return_y_)(const int, std::vector<double>&);
    
    std::string lpn_return_ydot_name_;
    void (*lpn_return_ydot_)(const int, std::vector<double>&);
    
    std::string lpn_set_external_step_size_name_;
    void (*lpn_set_external_step_size_)(const int, double);
#ifdef _WIN32
    dl_handle_t library_handle_ = nullptr;
#else
    void* library_handle_ = nullptr;
#endif
    int problem_id_ = 0;
    int system_size_ = 0;
    int num_cycles_ = 0;
    int pts_per_cycle_ = 0;
    int num_output_steps_ = 0;
    std::vector<std::string> block_names_;
    std::vector<std::string> variable_names_;
};

#endif

