#pragma once

#include <string>

class WAL
{
public:
    WAL() = default;
    ~WAL() = default;

    bool Append(const std::string &entry);
    bool Flush();
};
