
#ifndef COSIM_SSP_LOADER_HPP
#define COSIM_SSP_LOADER_HPP

#include "cosim/structure/simulation_structure.hpp"

#include <filesystem>
#include <memory>

namespace cosim
{

std::unique_ptr<simulation_structure> load_ssp(const std::filesystem::path& path);

} // namespace cosim

#endif // COSIM_SSP_LOADER_HPP
