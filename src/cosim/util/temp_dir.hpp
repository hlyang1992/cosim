
#ifndef COSIM_TEMP_DIR_HPP
#define COSIM_TEMP_DIR_HPP

#include <filesystem>

namespace cosim
{

class temp_dir
{
private:
    const std::filesystem::path path_;

public:
    explicit temp_dir(const std::string& name);

    temp_dir(temp_dir&&) = delete;
    temp_dir(const temp_dir&) = delete;
    temp_dir& operator=(temp_dir&&) = delete;
    temp_dir& operator=(const temp_dir&) = delete;


    [[nodiscard]] std::filesystem::path path() const noexcept
    {
        return path_;
    }

    ~temp_dir();
};

} // namespace cosim


#endif // COSIM_TEMP_DIR_HPP
