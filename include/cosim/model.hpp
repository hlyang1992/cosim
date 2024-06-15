
#ifndef COSIM_MODEL_HPP
#define COSIM_MODEL_HPP

#include "model_instance.hpp"

#include <memory>
#include <optional>

namespace cosim
{

class model
{

public:
    virtual std::unique_ptr<model_instance> instantiate(const std::string& instanceName, std::optional<double> stepSizeHint = std::nullopt) = 0;

    virtual ~model() = default;
};

} // namespace cosim

#endif // COSIM_MODEL_HPP
