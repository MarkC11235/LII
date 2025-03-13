#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <string>
#include <vector>

std::vector<std::string> find_test_files(const std::string& source_file, const std::string& extension = ".cl");

#endif // TEST_UTILS_HPP