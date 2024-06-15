
add_executable(cosim main.cpp)
target_link_libraries(cosim
        PRIVATE
        libcosim
        CLI11::CLI11
)
