
#ifndef COSIM_ALGORITHM_HPP
#define COSIM_ALGORITHM_HPP

#include "cosim/model_instance.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace cosim
{

struct algorithm
{

    virtual void model_instance_added(model_instance* instance) = 0;

    virtual void model_instance_removed(model_instance* instance){};

    virtual double step(double currentTime) = 0;

    virtual ~algorithm() = default;
};


} // namespace cosim

#endif // COSIM_ALGORITHM_HPP
