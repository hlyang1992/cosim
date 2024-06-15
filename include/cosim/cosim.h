
#ifndef LIBCOSIM_COSIM_H
#define LIBCOSIM_COSIM_H

#include <cstdbool>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

const char* cosim_last_error_msg();

typedef struct cosim_simulation cosim_simulation_t;
typedef struct cosim_simulation_listener cosim_simulation_listener_t;

void set_log_level(const char* level);

cosim_simulation_t* cosim_simulation_create(const char* sspPath, double stepSize);

bool cosim_simulation_init(cosim_simulation_t* sim, double startTime = 0, const char* parameterSet = nullptr);

void cosim_simulation_step(cosim_simulation_t* sim, size_t numSteps = 1);
void cosim_simulation_step_until(cosim_simulation_t* sim, double timePoint);

bool cosim_simulation_get_integer(cosim_simulation_t* sim, const char* identifier, int* value);
bool cosim_simulation_get_real(cosim_simulation_t* sim, const char* identifier, double* value);
bool cosim_simulation_get_bool(cosim_simulation_t* sim, const char* identifier, bool* value);
bool cosim_simulation_get_string(cosim_simulation_t* sim, const char* identifier, const char* value);

bool cosim_simulation_set_integer(cosim_simulation_t* sim, const char* identifier, int value);
bool cosim_simulation_set_real(cosim_simulation_t* sim, const char* identifier, double value);
bool cosim_simulation_set_bool(cosim_simulation_t* sim, const char* identifier, bool value);
bool cosim_simulation_set_string(cosim_simulation_t* sim, const char* identifier, const char* value);

void cosim_simulation_terminate(cosim_simulation_t* sim);

void cosim_simulation_add_listener(cosim_simulation_t* sim, const char* name, cosim_simulation_listener_t* listener);
void cosim_simulation_remove_listener(cosim_simulation_t* sim, const char* name);
cosim_simulation_listener_t* cosim_csv_writer_create(const char* resultFile, const char* logConfig = nullptr, const char* plotConfig = nullptr);

typedef struct cosim_simulation_info
{
    double time;
    size_t iterations;
} cosim_simulation_info;

typedef struct cosim_simulation_listener_config
{
    void (*preStepCallback)(cosim_simulation_info) = nullptr;
    void (*postStepCallback)(cosim_simulation_info) = nullptr;
} cosim_simulation_listener_config;

cosim_simulation_listener_t* cosim_simulation_listener_create(cosim_simulation_listener_config config);

void cosim_simulation_destroy(cosim_simulation_t* sim);


typedef struct cosim_version
{
    int major;
    int minor;
    int patch;
} cosim_version;

cosim_version cosim_library_version();


#ifdef __cplusplus
} // extern(C)
#endif

#endif // LIBCOSIM_COSIM_H
