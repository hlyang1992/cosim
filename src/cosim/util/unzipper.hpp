
#ifndef COSIM_UNZIPPER_HPP
#define COSIM_UNZIPPER_HPP

#include <filesystem>

namespace cosim
{

bool unzip(const std::filesystem::path& zip_file, const std::filesystem::path& tmp_path);

} // namespace cosim

#endif // COSIM_UNZIPPER_HPP
