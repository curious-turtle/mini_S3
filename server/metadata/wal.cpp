#include "wal.h"

bool WAL::Append(const std::string &entry)
{
    (void)entry;
    return false;
}

bool WAL::Flush()
{
    return false;
}
