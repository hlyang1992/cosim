
#include "vico/simulation.hpp"

#include "vico/listeners/simulation_listener.hpp"

using namespace vico;

simulation::simulation(std::unique_ptr<algorithm> algorithm)
    : algorithm_(std::move(algorithm))
{ }

void simulation::init(double startTime)
{
    if (!initialized) {

        initialized = true;

        for (auto& listener : listeners_) {
            listener->pre_init(*this);
        }

        for (auto& instance : instances_) {
            instance->setup_experiment(startTime);
            instance->enter_initialization_mode();
        }

        for (auto& instance : instances_) {
            instance->get_properties().applySets();
            instance->get_properties().applyGets();
        }

        for (auto& instance : instances_) {
            instance->exit_initialization_mode();
            instance->get_properties().applyGets();
        }

        for (auto& listener : listeners_) {
            listener->post_init(*this);
        }
    }
}

void simulation::step(unsigned int numStep)
{
    if (!initialized) { init(); }

    for (unsigned i = 0; i < numStep; i++) {

        for (auto& listener : listeners_) {
            listener->pre_step(*this);
        }

        double newT = algorithm_->step(currentTime, instances_);

        for (auto& c : connections_) {
            c->transferData();
        }

        currentTime = newT;

        for (auto& listener : listeners_) {
            listener->post_step(*this);
        }

        num_iterations++;
    }
}

void simulation::terminate()
{
    for (auto& instance : instances_) {
        instance->terminate();
    }

    for (auto& listener : listeners_) {
        listener->post_terminate(*this);
    }
}

void simulation::add_listener(std::unique_ptr<simulation_listener> listener)
{
    listeners_.emplace_back(std::move(listener));
}

model_instance* simulation::get_instance(const std::string& name)
{
    for (auto& instance : instances_) {
        if (instance->instanceName == name) {
            return instance.get();
        }
    }
    return nullptr;
}

void simulation::add_slave(std::unique_ptr<model_instance> instance)
{
    const auto name = instance->instanceName;
    if (get_instance(name)) {
        throw std::runtime_error("A model instance named '" + name + "' has already been added!");
    }

    instances_.emplace_back(std::move(instance));
}
