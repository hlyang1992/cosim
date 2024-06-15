
#ifndef LIBCOSIM_LIB_INFO_HPP
#define LIBCOSIM_LIB_INFO_HPP

namespace cosim
{

/// Software version
struct version
{
    int major = 0;
    int minor = 0;
    int patch = 0;
};

/// Returns the version of the cosim library.
version library_version();

} // namespace cosim

#endif // LIBCOSIM_LIB_INFO_HPP
