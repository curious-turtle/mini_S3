#pragma once

#include <string>
#include <memory>
#include <optional>

class ObjectStore
{
public:
    explicit ObjectStore(const std::string &root = "/tmp/server_object_store") noexcept;

    // Create a bucket. Returns true on success. If bucket already exists, returns false and sets err.
    bool put(const std::string &bucket, std::string &err);

    // Store an object in an existing bucket. If the object already exists, returns false and sets err.
    bool put(const std::string &bucket, const std::string &key, const std::string &data, std::string &err);

    // Retrieve an object. On success returns true and fills `out`. Otherwise returns false and sets err.
    bool get(const std::string &bucket, const std::string &key, std::string &out, std::string &err) const;

    // Root storage directory
    std::string root_path() const { return root_; }

private:
    std::string root_;
};
