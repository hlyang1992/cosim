# Ecos

>Ecos is a co-simulation engine.

Ecos (Easy co-simulation) is a fast, efficient and very easy to use co-simulation
engine written in modern C++.

Ecos provides the following features:
* FMI for Co-simulation version 1.0 & 2.0
* SSP version 1.0
* Optional sandboxed/remote model execution
* Post-simulation plotting using matplotlib
* Command-line-interface (CLI)
* Simplified Python and C interface

### Building

Ecos relies on [vcpkg](https://vcpkg.io/en/getting-started.html) to handle dependencies.

Once installed, please add the entry `VCPKG_ROOT` to `PATH` pointing your `/vcpkg` folder.
Alternatively, call CMake with `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`.

Example:

```
//windows
cmake . -A x64 -B build -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DECOS_BUILD_EXAMPLES=ON -DECOS_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

//linux
cmake . -B build -DCMAKE_TOOLCHAIN_FILE="/home/user/vcpkg/scripts/buildsystems/vcpkg.cmake" -DECOS_BUILD_EXAMPLES=ON -DECOS_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Notes on static linking and vcpkg (windows)

In order to produce executables with no DLL dependencies on windows, 
you need to call CMake with `-DVCPKG_TARGET_TRIPLET=x64-windows-static-md`

### Example

```cpp
using namespace ecos;

int main() {
    
    simulation_structure ss;

    // add models
    ss.add_model("chassis", "chassis.fmu");
    ss.add_model("ground", "ground.fmu");
    ss.add_model("wheel", "wheel.fmu");
    
    //make connections
    ss.make_connection<double>("chassis::p.e", "wheel.p1::e");
    ss.make_connection<double>("wheel::p1.f", "chassis.p::f");
    ss.make_connection<double>("wheel::p.e", "ground.p::e");
    ss.make_connection<double>("ground::p.f", "wheel.p::f");
    
    // setup initialValues
    std::map<variable_identifier, scalar_value> map;
    map["chassis::C.mChassis"] = 4000.0;
    ss.add_parameter_set("initialValues", map);
    
    auto sim = ss.load(std::make_unique<fixed_step_algorithm>(1.0 / 100), "initialValues");
    
    sim->init();
    sim->step_until(10);
    
    sim->terminate();
}

```

### SSP example

```cpp
using namespace ecos;

int main() {
    
    auto ss = load_ssp("quarter-truck.ssp");

    // use a fixed-step algorithm and apply parameterset from SSP file
    auto sim = ss->load(std::make_unique<fixed_step_algorithm>(1.0 / 100), "initialValues");
    
    // setup csv logging
    csv_config config;
    config.register_variable("chassis::zChassis"); // logs a single variable
    
    auto csvWriter = std::make_unique<csv_writer>("data.csv", config);
    csvWriter->enable_plotting("ChartConfig.xml"); // enable post-simulation plotting
    sim->add_listener(std::move(csvWriter));
    
    sim->init();
    sim->step_until(10);
    
    sim->terminate();
}
```

### Command line interface

```
Options:
  -h [ --help ]                 Print this help message and quits.
  -v [ --version ]              Print program version.
  -i [ --interactive ]          Make execution interactive.
  -l [ --logLevel ] arg (=info) Specify log level [trace,debug,info,warn,err,of
                                f].
  --path arg                    Location of the fmu/ssp to simulate.
  --stopTime arg (=1)           Simulation end.
  --startTime arg (=0)          Simulation start.
  --stepSize arg                Simulation stepSize.
  --rtf arg (=-1)               Target real time factor (non-positive number ->
                                inf).
  --noLog                       Disable CSV logging.
  --noParallel                  Run single-threaded.
  --logConfig arg               Path to logging configuration.
  --chartConfig arg             Path to chart configuration.
  --scenarioConfig arg          Path to scenario configuration.

```

### Python interface

To use the python interface, simply clone the project and run:

`pip install ./ecospy`

See [here](examples/quarter_truck/quarter_truck.py) for an example.

>Note: if using an old pip version, append `--use-feature=in-tree-build` if you get an error about `../version.txt`

---

### Compile-time requirements

* Windows (10 >=) or Ubuntu (20.04 >=) 
* C++17 compiler (MSVC >= 16 || gcc9 >=)
* CMake >= 3.17
* Vcpkg

#### Additional Linux requirements

`sudo apt install libtbb-dev`

### Run-time requirements
* Python3 (required for plotting)
  * matplotlib
  * pandas


---
> Want to build FMUs in C++? Check out [FMU4cpp](https://github.com/Vico-platform/fmu4cpp) </br>
> Want to build FMUs in Kotlin/Java? Check out [FMU4j](https://github.com/Vico-platform/FMU4j) </br>
> Want to build FMUs in Python? Check out [PythonFMU](https://github.com/NTNU-IHB/PythonFMU) </br>
> SSP generation made easy? Check out [SSPgen](https://github.com/Vico-platform/sspgen) </br>
