
#ifndef COSIM_SIMULATION_LISTENER_HPP
#define COSIM_SIMULATION_LISTENER_HPP


namespace cosim
{

class simulation;

struct simulation_listener
{

    virtual void pre_init(simulation& sim);
    virtual void post_init(simulation& sim);

    virtual void pre_step(simulation& sim);
    virtual void post_step(simulation& sim);

    virtual void post_terminate(simulation& sim);

    virtual void reset();

    virtual ~simulation_listener() = default;
};

} // namespace cosim

#endif // COSIM_SIMULATION_LISTENER_HPP
