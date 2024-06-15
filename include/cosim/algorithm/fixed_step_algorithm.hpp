
#ifndef COSIM_FIXED_STEP_ALGORITHM_HPP
#define COSIM_FIXED_STEP_ALGORITHM_HPP

#include "algorithm.hpp"

#include <algorithm>
#include <execution>

namespace cosim
{

struct fixed_step_algorithm : public algorithm
{

public:
    explicit fixed_step_algorithm(double stepSize, bool parallel = true);

    void model_instance_added(model_instance* instance) override;

    double step(double currentTime) override;

    ~fixed_step_algorithm() override;

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};


} // namespace cosim

#endif // COSIM_FIXED_STEP_ALGORITHM_HPP
