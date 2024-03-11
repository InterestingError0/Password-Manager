#pragma once
#include "cryptopp/cryptlib.h"
#include "cryptopp/filters.h"
#include <string>
#include <utility>

std::pair <std::string, std::string> hashMasterPassword(const std::string& masterPassword, std::string salt = "");
