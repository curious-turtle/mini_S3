// this file makes 100 files each of 1MB in size
// because we are in bazel we have to take care that output files don't go into sandbox
// so run it like: bazel run //client:generate_objects -- --data_dir=/tmp/object_store

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "generate_objects.h"

bool generate_test_files(const std::string &data_dir, size_t file_count, size_t file_size)
{
    try
    {
        std::filesystem::create_directories(data_dir);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create directory '" << data_dir << "': " << e.what() << std::endl;
        return false;
    }

    std::vector<char> buffer(file_size, 'A');

    for (size_t i = 0; i < file_count; ++i)
    {
        std::ostringstream ss;
        ss << data_dir << "/data_" << std::setw(3) << std::setfill('0') << i << ".bin";
        const std::string path = ss.str();

        std::ofstream ofs(path, std::ios::binary);
        if (!ofs)
        {
            std::cerr << "Failed to open file for writing: " << path << std::endl;
            return false;
        }

        ofs.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!ofs)
        {
            std::cerr << "Failed while writing to: " << path << std::endl;
            return false;
        }

        ofs.close();
        std::cout << "Wrote " << path << " (" << (file_size / 1024) << " KB)" << std::endl;
    }

    std::cout << "Done: created " << file_count << " files in " << data_dir << std::endl;
    return true;
}
