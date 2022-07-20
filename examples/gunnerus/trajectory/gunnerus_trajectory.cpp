#include "ecos/algorithm/fixed_step_algorithm.hpp"
#include "ecos/listeners/csv_writer.hpp"
#include "ecos/scenario/scenario_loader.hpp"
#include "ecos/ssp/ssp_loader.hpp"

#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

using namespace ecos;

int main()
{
    spdlog::set_level(spdlog::level::debug);

    const auto sspFile = std::string(SOURCE_DIR) + "/gunnerus-trajectory.ssp";
    if (!std::filesystem::exists(sspFile)) {
        spdlog::error("gunnerus-trajectory.ssp has not been generated yet. Run sspgen.");
        return -1;
    }

    try {
        auto ss = load_ssp(sspFile);
        auto sim = ss->load(std::make_unique<fixed_step_algorithm>(0.05));

        csv_config config;
        config.log_variable("vesselModel::cgShipMotion.nedDisplacement.north");
        config.log_variable("vesselModel::cgShipMotion.nedDisplacement.east");
        config.log_variable("vesselModel::cgShipMotion.nedDisplacement.down");

        auto csvWriter = std::make_unique<csv_writer>("results/gunnerus_trajectory.csv", config);
        csvWriter->enable_plotting(std::string(SOURCE_DIR) + "/ChartConfig.xml");
        sim->add_listener(std::move(csvWriter));

        load_scenario(*sim, std::string(SOURCE_DIR) + "/scenario.xml");

        spdlog::stopwatch sw;
        sim->init("initialValues");
        sim->step_until(250);
        spdlog::info("Elapsed {:.3}s", sw);
        sim->terminate();
    } catch (std::exception& ex) {
        std::cerr << "[ERROR]: " << ex.what() << std::endl;
    }
}