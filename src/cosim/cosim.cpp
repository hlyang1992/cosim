
#include "cosim/cosim.h"

#include "cosim/algorithm/fixed_step_algorithm.hpp"
#include "cosim/lib_info.hpp"
#include "cosim/listeners/csv_writer.hpp"
#include "cosim/logger/logger.hpp"
#include "cosim/simulation.hpp"
#include "cosim/ssp/ssp_loader.hpp"

#include <memory>

std::string g_last_error_msg;


const char* cosim_last_error_msg()
{
    return g_last_error_msg.c_str();
}

struct cosim_simulation
{
    std::unique_ptr<cosim::simulation> cpp_sim;
};

struct cosim_simulation_listener
{
    std::unique_ptr<cosim::simulation_listener> cpp_listener;
};

void handle_current_exception()
{
    try {
        throw;
    } catch (const std::exception& ex) {
        g_last_error_msg = ex.what();
    }
}

void set_log_level(const char* level)
{
    if (std::string("trace") == level) {
        cosim::log::set_logging_level(cosim::log::level::trace);
    } else if (std::string("debug") == level) {
        cosim::log::set_logging_level(cosim::log::level::debug);
    } else if (std::string("info") == level) {
        cosim::log::set_logging_level(cosim::log::level::info);
    } else if (std::string("warn") == level) {
        cosim::log::set_logging_level(cosim::log::level::warn);
    } else if (std::string("err") == level) {
        cosim::log::set_logging_level(cosim::log::level::err);
    } else if (std::string("off") == level) {
        cosim::log::set_logging_level(cosim::log::level::off);
    }
}


cosim_simulation_t* cosim_simulation_create(const char* sspPath, double stepSize)
{
    try {
        auto ss = cosim::load_ssp(sspPath);

        auto algorithm = std::make_unique<cosim::fixed_step_algorithm>(stepSize);
        auto sim = std::make_unique<cosim_simulation_t>();
        sim->cpp_sim = ss->load(std::move(algorithm));
        return sim.release();
    } catch (...) {
        handle_current_exception();
        return nullptr;
    }
}

bool cosim_simulation_init(cosim_simulation_t* sim, double startTime, const char* parameterSet)
{
    try {
        if (!parameterSet) {
            sim->cpp_sim->init(startTime);
        } else {
            sim->cpp_sim->init(startTime, parameterSet);
        }
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

void cosim_simulation_step(cosim_simulation_t* sim, size_t numSteps)
{
    sim->cpp_sim->step(numSteps);
}

void cosim_simulation_step_until(cosim_simulation_t* sim, double timePoint)
{
    sim->cpp_sim->step_until(timePoint);
}

bool cosim_simulation_get_integer(cosim_simulation_t* sim, const char* identifier, int* value)
{
    try {
        auto prop = sim->cpp_sim->get_int_property(identifier);
        *value = prop->get_value();
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_get_real(cosim_simulation_t* sim, const char* identifier, double* value)
{
    try {
        auto prop = sim->cpp_sim->get_real_property(identifier);
        *value = prop->get_value();
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_get_bool(cosim_simulation_t* sim, const char* identifier, bool* value)
{
    try {
        auto prop = sim->cpp_sim->get_bool_property(identifier);
        *value = prop->get_value();
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_get_string(cosim_simulation_t* sim, const char* identifier, const char* value)
{
    try {
        auto prop = sim->cpp_sim->get_string_property(identifier);
        value = prop->get_value().c_str();
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_set_integer(cosim_simulation_t* sim, const char* identifier, int value)
{
    try {
        auto prop = sim->cpp_sim->get_int_property(identifier);
        prop->set_value(value);
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_set_real(cosim_simulation_t* sim, const char* identifier, double value)
{
    try {
        auto prop = sim->cpp_sim->get_real_property(identifier);
        prop->set_value(value);
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_set_bool(cosim_simulation_t* sim, const char* identifier, bool value)
{
    try {
        auto prop = sim->cpp_sim->get_bool_property(identifier);
        prop->set_value(value);
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}

bool cosim_simulation_set_string(cosim_simulation_t* sim, const char* identifier, const char* value)
{
    try {
        auto prop = sim->cpp_sim->get_string_property(identifier);
        prop->set_value(value);
        return true;
    } catch (...) {
        handle_current_exception();
        return false;
    }
}


void cosim_simulation_add_listener(cosim_simulation_t* sim, const char* name, cosim_simulation_listener_t* listener)
{
    if (listener) {
        sim->cpp_sim->add_listener(name, std::move(listener->cpp_listener));
        delete listener;
        listener = nullptr;
    }
}

void cosim_simulation_remove_listener(cosim_simulation_t* sim, const char* name)
{
    if (name) {
        sim->cpp_sim->remove_listener(name);
    }
}

cosim_simulation_listener_t* cosim_csv_writer_create(const char* resultFile, const char* logConfig, const char* plotConfig)
{
    try {

        auto writer = std::make_unique<cosim::csv_writer>(resultFile);
        if (logConfig) {
            writer->config().load(logConfig);
        }
        if (plotConfig) {
            writer->config().enable_plotting(plotConfig);
        }

        auto l = std::make_unique<cosim_simulation_listener_t>();
        l->cpp_listener = std::move(writer);

        return l.release();

    } catch (...) {
        handle_current_exception();
        return nullptr;
    }
}

void cosim_simulation_terminate(cosim_simulation_t* sim)
{
    if (sim) {
        sim->cpp_sim->terminate();
    }
}

void cosim_simulation_destroy(cosim_simulation_t* sim)
{
    delete sim;
    sim = nullptr;
}


cosim_version cosim_library_version()
{
    cosim::version v = cosim::library_version();
    return {v.major, v.minor, v.patch};
}

class my_listener : public cosim::simulation_listener
{

public:
    explicit my_listener(cosim_simulation_listener_config config)
    {
        if (config.preStepCallback) {
            preStepCallback_ = config.preStepCallback;
        }
        if (config.postStepCallback) {
            postStepCallback_ = config.postStepCallback;
        }
    }

    void pre_step(cosim::simulation& sim) override
    {
        if (preStepCallback_) preStepCallback_->operator()(createInfo(sim));
    }

    void post_step(cosim::simulation& sim) override
    {
        if (postStepCallback_) postStepCallback_->operator()(createInfo(sim));
    }

private:
    std::optional<std::function<void(cosim_simulation_info)>> preStepCallback_;
    std::optional<std::function<void(cosim_simulation_info)>> postStepCallback_;

    static cosim_simulation_info createInfo(const cosim::simulation& sim)
    {
        return cosim_simulation_info{
            sim.time(),
            sim.iterations()};
    }
};

cosim_simulation_listener_t* cosim_simulation_listener_create(cosim_simulation_listener_config config)
{
    auto l = std::make_unique<cosim_simulation_listener_t>();
    l->cpp_listener = std::make_unique<my_listener>(config);

    return l.release();
}
