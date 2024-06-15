
#include "cosim/listeners/simulation_listener.hpp"

#include "cosim/simulation.hpp"

namespace cosim
{

void simulation_listener::pre_init(simulation& sim) { }
void simulation_listener::post_init(simulation& sim) { }

void simulation_listener::pre_step(simulation& sim) { }
void simulation_listener::post_step(simulation& sim) { }

void simulation_listener::post_terminate(simulation& sim) { }

void simulation_listener::reset() { }

} // namespace cosim
