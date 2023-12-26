#include "miscellaneousFunctions.hpp"
#include "cryptographyFunctions.hpp"
#include "cryptopp/cryptlib.h"
#include "cryptopp/secblock.h"
#include "cryptopp/osrng.h"
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <limits>
#include <utility>
#include "ncurses.h"

namespace fs = std::filesystem;

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
		if((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126)) {
			specialCharacterCount++;
		} else if(c >= 48 && c <= 57) {
			numberCount++;
		} else if(c >= 65 && c <= 90) {
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

void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt, const fs::path& masterPasswordPath) {
	std::ofstream oMasterPassword(masterPasswordPath, std::ios::binary);
	oMasterPassword << hashAndSalt.first << '\n' << hashAndSalt.second;
}

int menuChoice(const int lowerBound, const int upperBound) {
	int choice{ 0 };
	while((std::cout << "Enter your choice: " && !(std::cin >> choice)) || (choice < lowerBound || choice > upperBound)) {
		std::cout << "Invalid Input!\n\n";
		clearInputBuffer();
	}
	return choice;
}

bool checkIfEnteredMasterPasswordIsValid(const std::string& tempMasterPassword, const fs::path& masterPasswordPath) {
	std::ifstream iMasterPassword(masterPasswordPath, std::ios::binary);
	std::string masterPasswordHash;
	std::getline(iMasterPassword, masterPasswordHash);
	std::string salt;
	std::getline(iMasterPassword, salt);
	if(hash(tempMasterPassword, salt).first == masterPasswordHash) {
		return true;
	}
	return false;
}

std::string getFolderNameFromUser(const std::vector <std::string>& folders) {
	std::string folderName;
	while((std::cout << "Enter the folder name: ") && (std::cin >> folderName) && std::ranges::find(folders, folderName) == folders.end()) {
		std::cout << "Invalid Input!\n\n";
	}
	return folderName;
}

char generateCharacter() {
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::SecByteBlock gen(1);
	prng.GenerateBlock(gen, gen.size());
	const std::string character(reinterpret_cast <const char*>(gen.data()), gen.size());
	return character[0];
}
