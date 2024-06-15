
#ifndef COSIM_FMI_SCALAR_VARIABLE_HPP
#define COSIM_FMI_SCALAR_VARIABLE_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace fmilibcpp
{

template <typename  T>
struct attributes {
    std::optional<T> start;
};
using real_attributes = attributes<double>;
using integer_attributes = attributes<int>;
using string_attributes = attributes<std::string>;
using boolean_attributes = attributes<bool>;

using value_ref = unsigned int;
using type_attributes = std::variant<integer_attributes, real_attributes, string_attributes, boolean_attributes>;

inline std::string type_name(const type_attributes& attribute)
{
    switch (attribute.index()) {
        case 0: return "integer";
        case 1: return "real";
        case 2: return "string";
        case 3: return "boolean";
        default: throw std::runtime_error("Invalid variant");
    }
}

struct scalar_variable
{
    value_ref vr;
    std::string name;
    std::optional<std::string> causality;
    std::optional<std::string> variability;
    type_attributes typeAttributes;

    [[nodiscard]] bool is_integer() const
    {
        return std::holds_alternative<integer_attributes>(typeAttributes);
    }

    [[nodiscard]] bool is_real() const
    {
        return std::holds_alternative<real_attributes>(typeAttributes);
    }

    [[nodiscard]] bool is_string() const
    {
        return std::holds_alternative<string_attributes>(typeAttributes);
    }

    [[nodiscard]] bool is_boolean() const
    {
        return std::holds_alternative<boolean_attributes>(typeAttributes);
    }
};

using model_variables = std::vector<scalar_variable>;

} // namespace fmilibcpp

#endif // COSIM_FMI_SCALAR_VARIABLE_HPP
