#pragma once

#include <string>

class MetadataManager
{
public:
    MetadataManager();
    ~MetadataManager();

    bool LoadSnapshot(const std::string &path);
    bool ApplyWAL(const std::string &wal_path);
};
