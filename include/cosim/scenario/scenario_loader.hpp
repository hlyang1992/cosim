
#ifndef LIBCOSIM_SCENARIO_LOADER_HPP
#define LIBCOSIM_SCENARIO_LOADER_HPP

#include "cosim/simulation.hpp"

#include <filesystem>


namespace cosim
{

void load_scenario(simulation& sim, const std::filesystem::path& config);

}

#endif // LIBCOSIM_SCENARIO_LOADER_HPP
