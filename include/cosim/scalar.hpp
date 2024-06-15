
#ifndef LIBCOSIM_SCALAR_HPP
#define LIBCOSIM_SCALAR_HPP

#include <string>
#include <variant>

namespace cosim
{

using scalar_value = std::variant<double, int, bool, std::string>;

}


#endif // LIBCOSIM_SCALAR_HPP
