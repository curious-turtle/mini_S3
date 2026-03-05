#include "metadata_manager.h"

MetadataManager::MetadataManager() {}
MetadataManager::~MetadataManager() {}

bool MetadataManager::LoadSnapshot(const std::string &path)
{
    (void)path;
    return false;
}

bool MetadataManager::ApplyWAL(const std::string &wal_path)
{
    (void)wal_path;
    return false;
}
