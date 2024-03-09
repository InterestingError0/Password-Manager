#include "readFromFileFunctions.hpp"
#include "constants.hpp"
#include "cryptopp/aes.h"
#include "cryptopp/hex.h"
#include "cryptopp/filters.h"
#include "cryptopp/files.h"
#include <string>
#include <array>

std::string loadFile(CryptoPP::SecByteBlock& iv, const fs::path& filePath) {
    CryptoPP::FileSource fs(filePath.c_str(), false, new CryptoPP::HexDecoder(new CryptoPP::ArraySink(iv, iv.size())));
    fs.Pump(32);

    std::string encrypted;
    CryptoPP::FileSource fs2(filePath.c_str(), false, new CryptoPP::HexDecoder(new CryptoPP::StringSink(encrypted)));
    fs2.Skip(32);
    fs2.PumpAll();

    return encrypted;
}
