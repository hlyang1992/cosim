cmake_minimum_required(VERSION 3.17)
file(STRINGS "${CMAKE_SOURCE_DIR}/version.txt" projectVersion)
project("libcosim" VERSION ${projectVersion})
message("Current ${PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}\n")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/dist" CACHE PATH "Install prefix" FORCE)
set(CMAKE_INSTALL_DATADIR "${CMAKE_INSTALL_PREFIX}/data" CACHE PATH "Install data directory" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# ==============================================================================
# Build settings
# ==============================================================================

option(BUILD_SHARED_LIBS OFF)
option(COSIM_BUILD_TESTS "Build tests" OFF)
option(COSIM_BUILD_EXAMPLES "Build examples" OFF)
option(COSIM_BUILD_CLI "Build CLI" ON)
option(COSIM_BUILD_CLIB "Build C library" ON)

message(STATUS "Build shared libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "Build tests: ${COSIM_BUILD_TESTS}")
message(STATUS "Build examples: ${COSIM_BUILD_EXAMPLES}")
message(STATUS "Build CLI: ${COSIM_BUILD_CLI}")
message(STATUS "Build C library: ${COSIM_BUILD_CLIB}")


# ==============================================================================
# Global internal configuration
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(generatedSourcesDir "${CMAKE_BINARY_DIR}/generated")

# Use the highest warning levels and treat all warnings as errors, but ignore a
# few selected warnings.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options("-Wall" "-Wextra" "-Wpedantic")
  add_compile_options("-Wno-parentheses")
  # if(LIBCOSIM_TREAT_WARNINGS_AS_ERRORS) add_compile_options("-Werror") endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options("/W4")
  if(CMAKE_GENERATOR MATCHES "NMake Makefiles.*")
    string(REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  endif()
  add_compile_options("/wd4251" "/wd4996")
  # if(LIBCOSIM_TREAT_WARNINGS_AS_ERRORS) add_compile_options("/WX") endif()
  add_definitions("-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS")
endif()

# ==============================================================================
# Dependencies
# ==============================================================================

find_package(Thrift REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(libzip REQUIRED)
find_package(CLI11 REQUIRED)
find_package(fmilibrary REQUIRED)
find_package(pugixml REQUIRED)
find_package(nlohmann_json REQUIRED)

# ==============================================================================
# Targets
# ==============================================================================

add_subdirectory(src)
add_subdirectory(tool)

add_subdirectory(examples)
# if (COSIM_BUILD_EXAMPLES)
    # add_subdirectory(examples)
# endif ()

if (COSIM_BUILD_TESTS)
    find_package(Catch2 REQUIRED)
    enable_testing()
    add_subdirectory(tests)
endif ()


# ==============================================================================
# Exports and remaining installation
# ==============================================================================

# configure_package_config_file(cmake/config.cmake.in
#         ${CMAKE_CURRENT_BINARY_DIR}/cosim-config.cmake
#         INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cosim
#         NO_SET_AND_CHECK_MACRO)
# write_basic_package_version_file(
#         ${CMAKE_CURRENT_BINARY_DIR}/cosim-config-version.cmake
#         VERSION ${CMAKE_PROJECT_VERSION}
#         COMPATIBILITY SameMajorVersion)

# install(
#         FILES
#         ${CMAKE_CURRENT_BINARY_DIR}/cosim-config.cmake
#         ${CMAKE_CURRENT_BINARY_DIR}/cosim-config-version.cmake
#         DESTINATION
#         ${CMAKE_INSTALL_DATADIR}/cosim)

install(
        TARGETS libcosim libcosimc cosim proxyfmu
        EXPORT cosim-targets
)

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT cosim-targets
        NAMESPACE cosim::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cosim)