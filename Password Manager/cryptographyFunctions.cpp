#include "cryptographyFunctions.hpp"
#include "constants.hpp"
#include "cryptopp/scrypt.h"
#include "cryptopp/rijndael.h"
#include "cryptopp/secblock.h"
#include "cryptopp/osrng.h"
#include "cryptopp/gcm.h"
#include <string>
#include <utility>

std::pair <std::string, std::string> hashMasterPassword(const std::string& masterPassword, std::string salt) {
	if(salt.empty()) {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::byte gSalt[32];
		rng.GenerateBlock(gSalt, sizeof(gSalt));
        CryptoPP::StringSource(gSalt, sizeof(gSalt), true, new CryptoPP::StringSink(salt));
	}
	CryptoPP::Scrypt scrypt;
	CryptoPP::SecByteBlock derived(32);
	scrypt.DeriveKey(derived, derived.size(), reinterpret_cast <const CryptoPP::byte*>(masterPassword.data()), masterPassword.size(), reinterpret_cast <const CryptoPP::byte*>(salt.data()), salt.size(), 16384, 16, 16);
	std::string hash(reinterpret_cast<const char*>(&derived[0]), derived.size());

	return std::make_pair(hash, salt);
}

std::string encrypt(const std::string& plainText, CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv) {
	CryptoPP::GCM<CryptoPP::AES>::Encryption e;
	e.SetKeyWithIV(key, key.size(), iv, iv.size());

	std::string encrypted;

	CryptoPP::StringSource(plainText, true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(encrypted), false, tagSize));

	return encrypted;
}

std::string decrypt(const std::string& encrypted, CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv) {
	std::string decrypted;
	CryptoPP::GCM<CryptoPP::AES>::Decryption d;
	d.SetKeyWithIV(key, key.size(), iv, iv.size());
    CryptoPP::AuthenticatedDecryptionFilter df( d, new CryptoPP::StringSink(decrypted), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, tagSize);
    CryptoPP::StringSource( encrypted, true, new CryptoPP::Redirector(df));

    if(df.GetLastResult()) {
        return decrypted;
    }

	return "";
}
