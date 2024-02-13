#include "cryptographyFunctions.hpp"
#include "cryptopp/cryptlib.h"
#include "cryptopp/scrypt.h"
#include "cryptopp/secblock.h"
#include "cryptopp/osrng.h"
#include <string>
#include <utility>

std::pair <std::string, std::string> hash(const std::string& masterPassword, std::string salt) {
	if(salt.empty()) {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::byte gSalt[32];
		rng.GenerateBlock(gSalt, 32);
		CryptoPP::StringSource ss1(gSalt, 32, true, new CryptoPP::StringSink(salt));
	}
	CryptoPP::Scrypt scrypt;
	CryptoPP::SecByteBlock derived(32);
	scrypt.DeriveKey(derived, derived.size(), reinterpret_cast <const CryptoPP::byte*>(masterPassword.data()), masterPassword.size(), reinterpret_cast <const CryptoPP::byte*>(salt.data()), salt.size(), 16384, 16, 16);
	std::string hash;
	CryptoPP::StringSource ss2(derived, 32, true, new CryptoPP::StringSink(hash));
	return std::make_pair(hash, salt);
}
