#include "writeToFileFunctions.hpp"
#include "constants.hpp"
#include <string>
#include <array>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

void writeToFile(const std::string& data, const CryptoPP::SecByteBlock& iv, const fs::path& filePath) {
    std::string ivStr;
    CryptoPP::StringSource(iv, true, new CryptoPP::HexEncoder (new CryptoPP::StringSink(ivStr)));
    std::string encrypted;
	CryptoPP::StringSource(data, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encrypted)));

    CryptoPP::StringSource(ivStr + encrypted, true, new CryptoPP::FileSink(filePath.c_str()));
}
