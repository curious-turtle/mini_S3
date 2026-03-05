#include "object_store.h"
#include <filesystem>
#include <fstream>
#include <system_error>

namespace fs = std::filesystem;

ObjectStore::ObjectStore(const std::string &root) noexcept : root_(root)
{
    std::error_code ec;
    fs::create_directories(fs::path(root_), ec);
}

bool ObjectStore::put(const std::string &bucket, std::string &err)
{
    fs::path bpath = fs::path(root_) / bucket;
    std::error_code ec;
    if (fs::exists(bpath, ec))
    {
        if (fs::is_directory(bpath, ec))
        {
            err = "Bucket already exists";
            return false;
        }
        else
        {
            err = "A non-directory file with the bucket name exists";
            return false;
        }
    }

    if (!fs::create_directories(bpath, ec))
    {
        err = std::string("Failed to create bucket: ") + ec.message();
        return false;
    }

    return true;
}

bool ObjectStore::put(const std::string &bucket, const std::string &key, const std::string &data, std::string &err)
{
    fs::path bpath = fs::path(root_) / bucket;
    std::error_code ec;
    if (!fs::exists(bpath, ec) || !fs::is_directory(bpath, ec))
    {
        err = "Bucket does not exist";
        return false;
    }

    fs::path objpath = bpath / fs::path(key);
    if (fs::exists(objpath, ec))
    {
        err = "Object already exists";
        return false;
    }

    // Ensure parent directories for the object key exist
    if (objpath.has_parent_path())
    {
        fs::path parent = objpath.parent_path();
        if (!fs::exists(parent, ec))
        {
            if (!fs::create_directories(parent, ec))
            {
                err = std::string("Failed to create object parent directories: ") + ec.message();
                return false;
            }
        }
    }

    std::ofstream ofs(objpath, std::ios::binary);
    if (!ofs)
    {
        err = "Failed to open object file for writing";
        return false;
    }
    ofs.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!ofs)
    {
        err = "Failed to write object data";
        return false;
    }

    return true;
}

bool ObjectStore::get(const std::string &bucket, const std::string &key, std::string &out, std::string &err) const
{
    fs::path objpath = fs::path(root_) / bucket / fs::path(key);
    std::error_code ec;
    if (!fs::exists(objpath, ec) || !fs::is_regular_file(objpath, ec))
    {
        err = "Object not found";
        return false;
    }

    std::ifstream ifs(objpath, std::ios::binary);
    if (!ifs)
    {
        err = "Failed to open object file for reading";
        return false;
    }

    std::string data;
    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if (size > 0)
    {
        data.resize(static_cast<size_t>(size));
        ifs.read(&data[0], size);
        if (!ifs)
        {
            err = "Failed to read object data";
            return false;
        }
    }
    else
    {
        data.clear();
    }

    out = std::move(data);
    return true;
}
