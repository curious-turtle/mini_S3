#pragma once

#include <string>

// Generate `file_count` files of size `file_size` bytes under `data_dir`.
// Returns true on success, false on failure (and prints errors to stderr).
bool generate_test_files(const std::string &data_dir, size_t file_count = 100, size_t file_size = 1 << 20);
