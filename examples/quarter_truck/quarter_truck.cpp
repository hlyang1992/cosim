
#include "cosim/algorithm/fixed_step_algorithm.hpp"
#include "cosim/listeners/csv_writer.hpp"
#include "cosim/logger/logger.hpp"
#include "cosim/structure/simulation_structure.hpp"

using namespace cosim;

int main()
{
    log::set_logging_level(cosim::log::level::debug);

    simulation_structure ss;
    const std::filesystem::path fmuDir = std::string(DATA_FOLDER) + "/fmus/2.0/quarter-truck";

    try {
        ss.add_model("chassis", fmuDir / "chassis.fmu");
        ss.add_model("ground", fmuDir / "ground.fmu");
        ss.add_model("wheel", fmuDir / "wheel.fmu");

        ss.make_connection<double>("chassis::p.e", "wheel::p1.e");
        ss.make_connection<double>("wheel::p1.f", "chassis::p.f");
        ss.make_connection<double>("wheel::p.e", "ground::p.e");
        ss.make_connection<double>("ground::p.f", "wheel::p.f");

        std::map<variable_identifier, scalar_value> map;
        map["chassis::C.mChassis"] = 400.0;
        ss.add_parameter_set("initialValues", map);

        auto sim = ss.load(std::make_unique<fixed_step_algorithm>(1.0 / 100));
        auto p = sim->get_real_property("chassis::zChassis");

        auto csvWriter = std::make_unique<csv_writer>("results/quarter_truck_with_config.csv");
        csv_config& config = csvWriter->config();
        config.load("../../data/ssp/quarter_truck/LogConfig.xml");
        config.enable_plotting(std::string(DATA_FOLDER) + "/ssp/quarter_truck/ChartConfig.xml");
        sim->add_listener("csv_writer", std::move(csvWriter));

        sim->init("initialValues");
        sim->step_until(5);
        log::info("value={}", p->get_value());

        sim->terminate();
    } catch (const std::exception& ex) {

        log::err(ex.what());
    }
}
