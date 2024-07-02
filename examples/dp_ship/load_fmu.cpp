#include "fmilibcpp/fmu.hpp"
#include "cosim/logger/logger.hpp"
#include <fmt/core.h>


using namespace cosim;
using namespace fmilibcpp;


int main()
{
    log::set_logging_level(cosim::log::level::debug);

    const std::filesystem::path fmuDir = std::string(DATA_FOLDER) + "/fmus/dp_ship/DPController.fmu";

    auto fmu_model = loadFmu(fmuDir);
    const auto& d = (*fmu_model).get_model_description();

    fmt::print("Load finished.\n");

    fmt::print("guid: {}\n", d.guid);
    fmt::print("author {}\n", d.author);
    
    
    nlohmann::json j = d;
	fmt::print("to_json finished.\n");

    fmt::print("{}\n", j.dump(4));

    return 0;

}