#include "Functions.hpp"
#include "cryptlib.h"
#include "scrypt.h"
#include "secblock.h"
#include "osrng.h"
#include "base64.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>
#include <limits>
#include <utility>

namespace fs = std::filesystem;

void clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool checkIfPasswordMeetsRequirements(const std::string& masterPassword) {
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
	if(masterPassword.length() < 16) {
		std::cout << "Your master password is too short. The length of the master password must be greater than 16.\n";
		passwordMeetsRequirements = false;
	}
	if(!specialCharacterCount) {
		std::cout << "Your master password must contain a special character (e.g., !, @, #, $, etc.).\n";
		passwordMeetsRequirements = false;
	}
	if(!numberCount) {
		std::cout << "Your master password must contain a number (e.g., 1, 2, 3, 4, etc.).\n";
		passwordMeetsRequirements = false;
	}
	if(!uppercaseLetterCount) {
		std::cout << "Your master password must contain an uppercase letter (e.g., A, B, C, D, etc.).\n";
		passwordMeetsRequirements = false;
	}
	return passwordMeetsRequirements;
}

std::pair <std::string, std::string> hash(std::string& masterPassword, std::string salt) {
	std::string encoded;
	CryptoPP::Base64Encoder encoder;
	CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::Pad(), false)(CryptoPP::Name::InsertLineBreaks(), false);
	encoder.IsolatedInitialize(params);
	encoder.Attach(new CryptoPP::StringSink(encoded));
	if(salt.empty()) {
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::byte gSalt[32];
		rng.GenerateBlock(gSalt, 32);
		CryptoPP::StringSource ss1(gSalt, 32, true, new CryptoPP::Redirector(encoder));
		salt = encoded;
	}
	masterPassword += salt;
	CryptoPP::Scrypt scrypt;
	CryptoPP::SecByteBlock derived(32);
	scrypt.DeriveKey(derived, derived.size(), reinterpret_cast <const CryptoPP::byte*> (masterPassword.data()), masterPassword.size(), reinterpret_cast <const CryptoPP::byte*> (salt.data()), salt.size(), 16384, 16, 16);
	encoded = "";
	CryptoPP::StringSource ss2(derived, 32, true, new CryptoPP::Redirector(encoder));
	return std::make_pair(encoded, salt);
}

void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt, const fs::path& masterPasswordPath) {
	std::ofstream oMasterPassword(masterPasswordPath);
	oMasterPassword << hashAndSalt.first << '\n' << hashAndSalt.second;
}

int menuChoice(int lowerBound, int upperBound) {
	int choice{ 0 };
	while((std::cout << "Enter your choice: ") && !(std::cin >> choice) || (choice < lowerBound || choice > upperBound)) {
		std::cout << "Invalid Input!\n\n";
		clearInputBuffer();
	}
	return choice;
}

bool checkIfEnteredMasterPasswordIsValid(std::string& tempMasterPassword, const fs::path& masterPasswordPath) {
	std::ifstream iMasterPassword(masterPasswordPath);
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

void readFoldersIntoVector(std::vector <std::string>& folders, const fs::path& foldersPath) {
	std::ifstream iFolders(foldersPath);
	std::string folderName;
	while(std::getline(iFolders, folderName)) {
		folders.push_back(folderName);
	}
}

void readLoginsIntoVector(std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath) {
	std::ifstream iLogins(loginsPath);
	std::array <std::string, 4> login;
	while(std::getline(iLogins, login[0])) {
		std::getline(iLogins, login[1]);
		std::getline(iLogins, login[2]);
		std::getline(iLogins, login[3]);
		logins.push_back(login);
	}
}

void readPasswordGeneratorHistoryIntoVector(std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath) {
	std::ifstream iPasswordGeneratorHistory(passwordGeneratorHistoryPath);
	std::string generatedPassword;
	while(std::getline(iPasswordGeneratorHistory, generatedPassword)) {
		passwordGeneratorHistory.push_back(generatedPassword);
	}
}

void readSecureNotesIntoVector(std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath) {
	std::ifstream iSecureNotes(secureNotesPath);
	std::array <std::string, 3> secureNote;
	while(std::getline(iSecureNotes, secureNote[0])) {
		std::getline(iSecureNotes, secureNote[1]);
		std::getline(iSecureNotes, secureNote[2]);
		secureNotes.push_back(secureNote);
	}
}

void writeFoldersToFile(const std::vector <std::string>& folders, const fs::path& foldersPath) {
	std::ofstream oFolders(foldersPath);
	for(const std::string& x : folders) {
		oFolders << x << '\n';
	}
}

void writeLoginsToFile(const std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath) {
	std::ofstream oLogins(loginsPath);
	for(const auto& x : logins) {
		for(const std::string& y : x) {
			oLogins << y << '\n';
		}
	}
}

void writePasswordGeneratorHistoryToFile(const std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath) {
	std::ofstream oPasswordManagerHistory(passwordGeneratorHistoryPath);
	for(const std::string& x : passwordGeneratorHistory) {
		oPasswordManagerHistory << x << '\n';
	}
}

void writeSecureNotesToFile(const std::vector<std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath) {
	std::ofstream oSecureNotes(secureNotesPath);
	for(const auto& x : secureNotes) {
		for(const std::string& y : x) {
			oSecureNotes << y << '\n';
		}
	}
}

bool deleteLoginIfLoginExists(std::vector <std::array <std::string, 4>>& logins, const std::string& webAddress, const std::string& username) {
	for(auto it{ logins.begin() }; it != logins.end(); ++it) {
		if((*it)[1] == webAddress && (*it)[2] == username) {
			logins.erase(it);
			return true;
		}
	}
	return false;
}

void deletePasswordGeneratorHistory(std::vector<std::string>& passwordGeneratorHistory) {
	passwordGeneratorHistory.resize(0);
}

bool deleteSecureNoteIfSecureNoteExists(std::vector <std::array <std::string, 3>>& secureNotes, const std::string& title) {
	for(auto it{ secureNotes.begin() }; it != secureNotes.end(); ++it) {
		if((*it)[1] == title) {
			secureNotes.erase(it);
			return true;
		}
	}
	return false;
}

void deleteFolder(std::vector <std::string>& folders, const std::string& deleteFolderName) {
	for(int i{ 0 }; i < folders.size(); i++) {
		if(folders[i] == deleteFolderName) {
			folders.erase(folders.begin() + i);
			break;
		}
	}
}

bool checkIfLoginIsDuplicate(const std::vector <std::array <std::string, 4>>& logins, const std::string& webAddress, const std::string& username) {
	for(int i{ 0 }; i < logins.size(); i++) {
		if(logins[i][1] == webAddress && logins[i][2] == username) {
			return true;
		}
	}
	return false;
}

bool checkIfSecureNoteIsDuplicate(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string& title) {
	for(int i{ 0 }; i < secureNotes.size(); i++) {
		if(secureNotes[i][1] == title) {
			return true;
		}
	}
	return false;
}

bool checkIfFolderIsDuplicate(const std::vector <std::string>& folders, const std::string& folder) {
	if(std::ranges::find(folders, folder) != folders.end()) {
		return true;
	}
	return false;
}

void printAllLogins(const std::vector <std::array <std::string, 4>>& logins) {
	std::cout << "Here are the logins stored in your vault:\n\n";
	for(int i{ 0 }; i < logins.size(); i++) {
		std::cout << "Folder: " << logins[i][0] << "\nWeb Address: " << logins[i][1] << "\nUsername: " << logins[i][2] << "\nPassword: " << logins[i][3] << "\n\n";
	}
}

bool printLoginsInFolder(const std::vector <std::array <std::string, 4>>& logins, const std::string& folder) {
	bool noLoginsInFolder{ true };
	for(int i{ 0 }; i < logins.size(); i++) {
		if(logins[i][0] == folder) {
			noLoginsInFolder = false;
			std::cout << "\nWeb Address: " << logins[i][1] << "\nUsername: " << logins[i][2] << "\nPassword: " << logins[i][3] << "\n\n";
		}
	}
	return noLoginsInFolder;
}

void printPasswordGeneratorHistory(const std::vector <std::string>& passwordGeneratorHistory) {
	std::cout << '\n';
	for(const std::string& x : passwordGeneratorHistory) {
		std::cout << x << '\n';
	}
}

void printAllSecureNotes(const std::vector <std::array <std::string, 3>>& secureNotes) {
	std::cout << "Here are the secure notes stored in your vault:\n\n";
	for(int i{ 0 }; i < secureNotes.size(); i++) {
		std::cout << "Folder: " << secureNotes[i][0] << "\nTitle: " << secureNotes[i][1] << "\nContent: " << secureNotes[i][2] << "\n\n";
	}
}

bool printSecureNotesInFolder(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string& folder) {
	bool noSecureNotesInFolder{ true };
	for(int i{ 0 }; i < secureNotes.size(); i++) {
		noSecureNotesInFolder = false;
		if(secureNotes[i][0] == folder) {
			std::cout << "Title: " << secureNotes[i][1] << "\nContent: " << secureNotes[i][2] << "\n\n";
		}
	}
	return noSecureNotesInFolder;
}

void printFolders(const std::vector <std::string>& folders) {
	std::cout << "Here are the folders currently available:\n\n";
	for(const std::string& folder : folders) {
		std::cout << folder << '\n';
	}
	std::cout << '\n';
}

char generateCharacter() {
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::SecByteBlock gen(1);
	prng.GenerateBlock(gen, gen.size());
	const std::string character(reinterpret_cast<const char*>(gen.data()), gen.size());
	return character[0];
}
