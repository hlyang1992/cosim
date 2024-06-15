
#ifndef LIBCOSIM_FMI_MODEL_SUB_RESOLVER_HPP
#define LIBCOSIM_FMI_MODEL_SUB_RESOLVER_HPP

#include "cosim/model_resolver.hpp"

namespace cosim
{

class fmi_model_sub_resolver : public model_sub_resolver
{
public:
    std::unique_ptr<model> resolve(const std::filesystem::path& base, const std::string& uri) override;
};

} // namespace cosim

#endif // LIBCOSIM_FMI_MODEL_SUB_RESOLVER_HPP
