#include "readFromFileFunctions.hpp"
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>

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
