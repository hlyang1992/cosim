
#ifndef VICO_VARIABLE_IDENTIFIER_HPP
#define VICO_VARIABLE_IDENTIFIER_HPP

#include <stdexcept>
#include <string>
#include <utility>

namespace vico
{

struct variable_identifier
{

    const std::string instanceName;
    const std::string variableName;

    variable_identifier(const std::string& identifier)
        : variable_identifier(parse(identifier))
    { }

    variable_identifier(const std::pair<std::string, std::string>& identifier)
        : instanceName(identifier.first)
        , variableName(identifier.second)
    { }

private:
    static std::pair<std::string, std::string> parse(const std::string& identifier)
    {
        auto result = identifier.find('.');
        if (result == std::string::npos) {
            throw std::runtime_error("Error parsing variable identifier. A '.' must be present!");
        }

        return {identifier.substr(0, result), identifier.substr(result + 1)};
    }
};

} // namespace vico

#endif // VICO_VARIABLE_IDENTIFIER_HPP
