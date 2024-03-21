#include "miscellaneousFunctions.hpp"
#include "constants.hpp"
#include "cryptographyFunctions.hpp"
#include "cryptopp/secblock.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"
#include <iostream>
#include <string>
#include <string_view>
#include <limits>
#include <utility>
#include "ncurses.h"

void clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
}

std::string getMasterPasswordFromUser() {
	noecho();
	cbreak();
	std::string masterPassword;
	char inputChar;
	while((inputChar = static_cast <char>(getch())) != '\n') {
		if((inputChar == 127 || inputChar == 8) && !masterPassword.empty()) {
			masterPassword.pop_back();
			printw("\b \b");
			continue;
		}
		if((inputChar == 127 || inputChar == 8) && masterPassword.empty()) {
			continue;
		}
        masterPassword.push_back(inputChar);
        printw("*");
	}
	printw("\n");
	nocbreak();
	return masterPassword;
}

bool checkIfPasswordMeetsRequirements(const std::string_view masterPassword) {
	int specialCharacterCount{ 0 };
	int numberCount{ 0 };
	int uppercaseLetterCount{ 0 };
	bool passwordMeetsRequirements{ true };

	for(const char c : masterPassword) {
		if(std::ranges::find(specialCharacters, c) != specialCharacters.end()) {
			specialCharacterCount++;
		} else if(std::ranges::find(numbers, c) != numbers.end()) {
			numberCount++;
		} else if(std::ranges::find(uppercaseLetters, c) != uppercaseLetters.end()) {
			uppercaseLetterCount++;
		}
	}

	if(masterPassword.length() < 8) {
		printw("Your master password is too short. The length of the master password must be greater than 8.\n");
		passwordMeetsRequirements = false;
	}
	if(!specialCharacterCount) {
		printw("Your master password must contain at least one special character (e.g., !, @, #, $, etc.).\n");
		passwordMeetsRequirements = false;
	}
	if(!numberCount) {
		printw("Your master password must contain at least one number (e.g., 1, 2, 3, 4, etc.).\n");
		passwordMeetsRequirements = false;
	}
	if(!uppercaseLetterCount) {
		printw("Your master password must contain at least one uppercase letter (e.g., A, B, C, D, etc.).\n");
		passwordMeetsRequirements = false;
	}
	return passwordMeetsRequirements;
}

void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt) {
    std::string hashEncoded;
    std::string saltEncoded;
    CryptoPP::StringSource(hashAndSalt.second, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(saltEncoded)));
    CryptoPP::StringSource(hashAndSalt.first, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashEncoded)));

    CryptoPP::StringSource(saltEncoded + hashEncoded, true, new CryptoPP::FileSink(masterPasswordPath.c_str()));
}

int menuChoice(const int upperBound) {
	int choice{ 0 };
	while((std::cout << "Enter your choice: " && !(std::cin >> choice)) || (choice < 1 || choice > upperBound)) {
		std::cout << "Invalid Input!\n\n";
		clearInputBuffer();
	}
	return choice;
}

bool checkIfEnteredMasterPasswordIsValid(const std::string& tempMasterPassword) {
    std::string hash;
    CryptoPP::FileSource fs(masterPasswordPath.c_str(), false, new CryptoPP::HexDecoder(new CryptoPP::StringSink(hash)));
    fs.Pump(64);
    const std::string salt{ hash };
    hash.clear();
    fs.PumpAll();

	if(hashMasterPassword(tempMasterPassword, salt).first == hash) {
		return true;
	}
	return false;
}

char generateCharacter() {
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::SecByteBlock gen(1);
	prng.GenerateBlock(gen, gen.size());
	const std::string character(reinterpret_cast <const char*>(gen.data()), gen.size());
	return character[0];
}
