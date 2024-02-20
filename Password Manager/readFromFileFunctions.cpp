#include "readFromFileFunctions.hpp"
#include "constants.hpp"
#include "cryptopp/hex.h"
#include "cryptopp/filters.h"
#include "cryptopp/files.h"
#include <string>
#include <array>

std::string loadFile(const fs::path& filePath) {
	std::string cipherText;
	CryptoPP::FileSource(filePath.c_str(), true, new CryptoPP::StringSink(cipherText));

	return cipherText;
}
