
#ifndef COSIM_FMI_FMI1_MODEL_DESCRIPTION_HPP
#define COSIM_FMI_FMI1_MODEL_DESCRIPTION_HPP

#include "fmilibcpp/model_description.hpp"

#include <fmilib.h>

namespace fmilibcpp
{

model_description create_model_description(fmi1_import_t* handle);

} // namespace fmilibcpp

#endif // COSIM_FMI_FMI1_MODEL_DESCRIPTION_HPP
