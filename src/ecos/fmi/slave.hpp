
#ifndef ECOS_FMI_SLAVE_HPP
#define ECOS_FMI_SLAVE_HPP

#include "model_description.hpp"

#include <memory>
#include <vector>

namespace ecos::fmi
{

using value_ref = unsigned int;

class slave
{
public:
    const std::string instanceName;

    explicit slave(std::string instanceName)
        : instanceName(std::move(instanceName))
    { }

    virtual bool setup_experiment(
        double start_time = 0,
        double stop_time = 0,
        double tolerance = 0) = 0;

    [[nodiscard]] virtual const model_description& get_model_description() const = 0;

    virtual bool enter_initialization_mode() = 0;
    virtual bool exit_initialization_mode() = 0;

    virtual bool step(double current_time, double step_size) = 0;

    virtual bool terminate() = 0;
    virtual bool reset() = 0;
    virtual void freeInstance() = 0;

    int get_integer(value_ref vr)
    {
        std::vector<int> values(1);
        get_integer({vr}, values);
        return values.front();
    }

    double get_real(value_ref vr)
    {
        std::vector<double> values(1);
        get_real({vr}, values);
        return values.front();
    }

    std::string get_string(value_ref vr)
    {
        std::vector<std::string> values(1);
        get_string({vr}, values);
        return values.front();
    }

    bool get_boolean(value_ref vr)
    {
        std::vector<bool> values(1);
        get_boolean({vr}, values);
        return values.front();
    }

    virtual bool get_integer(const std::vector<value_ref>& vrs, std::vector<int>& values) = 0;
    virtual bool get_real(const std::vector<value_ref>& vrs, std::vector<double>& values) = 0;
    virtual bool get_string(const std::vector<value_ref>& vrs, std::vector<std::string>& values) = 0;
    virtual bool get_boolean(const std::vector<value_ref>& vrs, std::vector<bool>& values) = 0;

    virtual bool set_integer(const std::vector<value_ref>& vrs, const std::vector<int>& values) = 0;
    virtual bool set_real(const std::vector<value_ref>& vrs, const std::vector<double>& values) = 0;
    virtual bool set_string(const std::vector<value_ref>& vrs, const std::vector<std::string>& values) = 0;
    virtual bool set_boolean(const std::vector<value_ref>& vrs, const std::vector<bool>& values) = 0;

    virtual ~slave() = default;
};

} // namespace fmi

#endif // ECOS_FMI_SLAVE_HPP
