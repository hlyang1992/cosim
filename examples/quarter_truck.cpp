#include <vico/algorithm/fixed_step_algorithm.hpp>
#include <vico/fmi/fmi_model.hpp>
#include <vico/structure/simulation_structure.hpp>
#include <vico/listeners/csv_writer.hpp>

#include <iostream>

using namespace vico;

int main()
{

    simulation_structure ss;

    ss.add_model("chassis", std::make_shared<fmi_model>("../data/fmus/2.0/quarter-truck/chassis.fmu"));
    ss.add_model("ground", std::make_shared<fmi_model>("../data/fmus/2.0/quarter-truck/ground.fmu"));
    ss.add_model("wheel", std::make_shared<fmi_model>("../data/fmus/2.0/quarter-truck/wheel.fmu"));

    ss.make_connection<double>("chassis.p.e", "wheel.p1.e");
    ss.make_connection<double>("wheel.p1.f", "chassis.p.f");
    ss.make_connection<double>("wheel.p.e", "ground.p.e");
    ss.make_connection<double>("ground.p.f", "wheel.p.f");

    std::map<variable_identifier, std::variant<double, int, bool, std::string>> map;
    map[variable_identifier{"chassis.C.mChassis"}] = 4000.0;
    ss.add_parameter_set("initialValues", map);

    auto sim = ss.load(std::make_unique<fixed_step_algorithm>(1.0 / 100));
    auto p = sim->get_real_property("chassis.zChassis");

    const auto& config = csv_config::parse("../data/fmus/2.0/quarter-truck/LogConfig.xml");
    sim->add_listener(std::make_unique<csv_writer>("results/quarter_truck_with_config.csv", config));

    sim->init("initialValues");
    while (sim->time() < 1) {
        sim->step();
        std::cout << p->operator()() << std::endl;
    }

    sim->terminate();
}
