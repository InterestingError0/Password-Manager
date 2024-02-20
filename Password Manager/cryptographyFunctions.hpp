#pragma once
#include "cryptopp/cryptlib.h"
#include "cryptopp/filters.h"
#include <string>
#include <utility>

std::pair <std::string, std::string> hashMasterPassword(const std::string& masterPassword, std::string salt = "");

std::string encrypt(std::string decrypted, CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv);
std::string decrypt(std::string encrypted, CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv);