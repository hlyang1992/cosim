
add_subdirectory(proxyfmu)

set(publicHeaders

        "cosim/connection.hpp"
        "cosim/lib_info.hpp"
        "cosim/model.hpp"
        "cosim/model_instance.hpp"
        "cosim/model_resolver.hpp"
        "cosim/property.hpp"
        "cosim/scalar.hpp"
        "cosim/scenario.hpp"
        "cosim/scenario_loader.hpp"
        "cosim/simulation.hpp"
        "cosim/simulation_runner.hpp"
        "cosim/variable_identifier.hpp"

        "cosim/algorithm/algorithm.hpp"
        "cosim/algorithm/fixed_step_algorithm.hpp"

        "cosim/listeners/simulation_listener.hpp"
        "cosim/listeners/csv_writer.hpp"

        "cosim/logger/logger.hpp"

        "cosim/structure/simulation_structure.hpp"

        "cosim/ssp/ssp_loader.hpp"

)

set(publicHeadersFull)
foreach (header IN LISTS commonPublicHeaders)
    list(APPEND commonPublicHeadersFull "${PROJECT_SOURCE_DIR}/include/${header}")
endforeach ()

set(privateHeaders

        "cosim/fmi/fmi_model.hpp"
        "cosim/fmi/fmi_model_instance.hpp"
        "cosim/fmi/fmi_model_sub_resolver.hpp"
        "cosim/fmi/proxy/proxy_model.hpp"
        "cosim/fmi/proxy/proxy_model_sub_resolver.hpp"

        "cosim/ssp/ssp.hpp"

        "cosim/util/temp_dir.hpp"
        "cosim/util/unzipper.hpp"
        "cosim/util/uuid.hpp"

        "fmilibcpp/buffered_slave.hpp"
        "fmilibcpp/fmicontext.hpp"
        "fmilibcpp/fmu.hpp"
        "fmilibcpp/model_description.hpp"
        "fmilibcpp/scalar_variable.hpp"
        "fmilibcpp/slave.hpp"

        "fmilibcpp/fmi1/fmi1_fmu.hpp"
        "fmilibcpp/fmi1/fmi1_slave.hpp"
        "fmilibcpp/fmi1/fmi1_model_description.hpp"

        "fmilibcpp/fmi2/fmi2_fmu.hpp"
        "fmilibcpp/fmi2/fmi2_slave.hpp"
        "fmilibcpp/fmi2/fmi2_model_description.hpp"

        "proxyfmu/process_helper.hpp"
        "proxyfmu/proxy_fmu.hpp"
        "proxyfmu/proxy_slave.hpp"
        "proxyfmu/remote_info.hpp"

)

set(sources

        "cosim/model_resolver.cpp"
        "cosim/simulation.cpp"
        "cosim/simulation_runner.cpp"

        "cosim/scenario/scenario.cpp"
        "cosim/scenario/scenario_loader_xml.cpp"

        "cosim/algorithm/fixed_step_algorithm.cpp"

        "cosim/fmi/fmi_model_sub_resolver.cpp"
        "cosim/fmi/proxy/proxy_model_sub_resolver.cpp"

        "cosim/listeners/csv_writer.cpp"
        "cosim/listeners/simulation_listener.cpp"

        "cosim/ssp/ssp.cpp"
        "cosim/ssp/ssp_loader.cpp"

        "cosim/structure/simulation_structure.cpp"

        "proxyfmu/proxy_fmu.cpp"
        "proxyfmu/proxy_slave.cpp"

)

# Generate sources from .in files
set(generatedFiles "${generatedSourcesDir}/cosim/lib_info.cpp")
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cosim/lib_info.cpp.in"
        "${generatedFiles}"
        @ONLY
)

add_library(logger OBJECT "cosim/logger/logger.cpp")
set_target_properties(logger PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(logger PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(logger
        PUBLIC
        fmt::fmt
        PRIVATE
        spdlog::spdlog
)

add_library(util OBJECT
        "cosim/util/temp_dir.cpp"
        "cosim/util/unzipper.cpp"
        "cosim/util/uuid.cpp"
)
set_target_properties(util PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(util PRIVATE libzip::zip fmt::fmt)
target_include_directories(util
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src")

add_library(fmilibcpp OBJECT
        "fmilibcpp/fmu.cpp"
        "fmilibcpp/model_description.cpp"
        "fmilibcpp/fmi1/fmi1_fmu.cpp"
        "fmilibcpp/fmi1/fmi1_slave.cpp"
        "fmilibcpp/fmi1/fmi1_model_description.cpp"
        "fmilibcpp/fmi2/fmi2_fmu.cpp"
        "fmilibcpp/fmi2/fmi2_slave.cpp"
        "fmilibcpp/fmi2/fmi2_model_description.cpp"
)
set_target_properties(fmilibcpp PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(fmilibcpp PRIVATE  fmilibrary::fmilibrary fmt::fmt nlohmann_json::nlohmann_json)
target_include_directories(fmilibcpp PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")

add_library(libcosim ${publicHeadersFull} ${privateHeaders} ${sources} ${generatedFiles})
set_target_properties(libcosim PROPERTIES PREFIX "")
set_target_properties(libcosim PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(libcosim PUBLIC "cxx_std_17")
target_include_directories(libcosim
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
        "${generatedSourcesDir}"
        "${SUBPROCESS_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(libcosim
        PUBLIC
        fmt::fmt
        PRIVATE
        libzip::zip
        spdlog::spdlog
        thrift::thrift
        pugixml::pugixml
        fmilibrary::fmilibrary
        nlohmann_json::nlohmann_json
        "$<TARGET_OBJECTS:util>"
        "$<TARGET_OBJECTS:logger>"
        "$<TARGET_OBJECTS:fmilibcpp>"
        "$<TARGET_OBJECTS:proxyfmu-service>"
)
if (UNIX)
    target_link_libraries(libcosim INTERFACE stdc++fs tbb)
endif ()

if (COSIM_BUILD_CLIB)
    add_library(libcosimc SHARED "${CMAKE_CURRENT_SOURCE_DIR}/cosim/cosim.cpp")
    set_target_properties(libcosimc PROPERTIES PREFIX "")
    set_property(TARGET libcosimc PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(libcosimc
            PUBLIC
            libcosim)
endif ()

# Make sure executable is up to date
add_dependencies(libcosim proxyfmu)
