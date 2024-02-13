#pragma once
#include <string>
#include <utility>

std::pair <std::string, std::string> hash(const std::string& masterPassword, std::string salt = "");
