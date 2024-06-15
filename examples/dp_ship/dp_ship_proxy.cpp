#include "cosim/algorithm/fixed_step_algorithm.hpp"
#include "cosim/listeners/csv_writer.hpp"
#include "cosim/logger/logger.hpp"
#include "cosim/structure/simulation_structure.hpp"
#include "cosim/model_resolver.hpp"

using namespace cosim;

int main(){
    log::set_logging_level(cosim::log::level::debug);

    simulation_structure ss;
    const std::filesystem::path fmuDir = "proxyfmu://localhost?file=" + std::string(DATA_FOLDER) + "/fmus/dp_ship";
    try {
        log::info("fmuDir={}", fmuDir.string());
        ss.add_model("DPController", (fmuDir / "DPController.fmu").string());
        ss.add_model("Observer", (fmuDir / "NLPobserver.fmu").string());
        ss.add_model("ReferenceGenerator", (fmuDir / "ReferenceGenerator.fmu").string());
        ss.add_model("ThrustAllocation", (fmuDir / "ThMPC.fmu").string());
        ss.add_model("Ship", (fmuDir / "OSOM.fmu").string());

        ss.make_connection<double>("ThrustAllocation::F1c", "Ship::Thrust_d[1]");
        ss.make_connection<double>("ThrustAllocation::F2c", "Ship::Thrust_d[2]");
        ss.make_connection<double>("ThrustAllocation::F3c", "Ship::Thrust_d[3]");

        ss.make_connection<double>("Observer::y_hat[1]", "DPController::x");
        ss.make_connection<double>("Observer::y_hat[2]", "DPController::y");
        ss.make_connection<double>("Observer::y_hat[3]", "DPController::psi");
        ss.make_connection<double>("Observer::v_hat[1]", "DPController::dx");
        ss.make_connection<double>("Observer::v_hat[2]", "DPController::dy");
        ss.make_connection<double>("Observer::v_hat[3]", "DPController::dpsi");

        ss.make_connection<double>("Ship::q[1]", "Observer::y[1]");
        ss.make_connection<double>("Ship::q[2]", "Observer::y[2]");
        ss.make_connection<double>("Ship::q[3]", "Observer::y[3]");

        ss.make_connection<double>("ThrustAllocation::Fxg", "Observer::tau[1]");
        ss.make_connection<double>("ThrustAllocation::Fyg", "Observer::tau[2]");
        ss.make_connection<double>("ThrustAllocation::Mzg", "Observer::tau[3]");

        ss.make_connection<double>("ThrustAllocation::alpha1", "Ship::input[1]");
        ss.make_connection<double>("ThrustAllocation::alpha2", "Ship::input[2]");

        ss.make_connection<double>("ReferenceGenerator::x_ref", "DPController::x_ref");
        ss.make_connection<double>("ReferenceGenerator::y_ref", "DPController::y_ref");
        ss.make_connection<double>("ReferenceGenerator::psi_ref", "DPController::psi_ref");

        ss.make_connection<double>("ReferenceGenerator::dx_ref", "DPController::dx_ref");
        ss.make_connection<double>("ReferenceGenerator::dy_ref", "DPController::dy_ref");
        ss.make_connection<double>("ReferenceGenerator::dpsi_ref", "DPController::dpsi_ref");


        std::map<variable_identifier, scalar_value> map {};
        // // map["chassis::C.mChassis"] = 400.0;
        // ss.add_parameter_set("initialValues", map);

        auto sim = ss.load(std::make_unique<fixed_step_algorithm>(1.0 / 100));

        auto csvWriter = std::make_unique<csv_writer>("results/dp_ship.csv");
        csv_config& config = csvWriter->config();
        config.register_variable("ThrustAllocation::F1c");
        config.register_variable("ThrustAllocation::F2c");
        config.register_variable("ThrustAllocation::F3c");
        config.register_variable("Ship::q[1]");
        config.register_variable("Ship::q[2]");
        config.register_variable("Ship::q[3]");
        sim->add_listener("csv_writer", std::move(csvWriter));

        sim->init();
        // sim->init("initialValues");
        sim->step_until(5);

        sim->terminate();
    } catch (const std::exception& ex) {

        log::err(ex.what());
    }
}
