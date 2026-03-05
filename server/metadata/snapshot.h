#pragma once

#include <string>

class Snapshot
{
public:
    Snapshot() = default;
    ~Snapshot() = default;

    bool Save(const std::string &path);
    bool Load(const std::string &path);
};
