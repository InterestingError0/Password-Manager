#pragma once
#include <string>
#include <utility>

std::pair <std::string, std::string> hash(std::string masterPassword, std::string salt = "");
