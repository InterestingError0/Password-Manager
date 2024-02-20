#include "writeToFileFunctions.hpp"
#include "constants.hpp"
#include <string>
#include <array>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

void writeToFile(const std::string& data, const fs::path& filePath) {
	CryptoPP::StringSource(data, true, new CryptoPP::FileSink(filePath.c_str()));
}
