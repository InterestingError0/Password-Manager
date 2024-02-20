#include "deleteFunctions.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <array>

bool deleteLoginIfLoginExists(std::vector <std::vector <std::string>>& logins, const std::string_view webAddress, const std::string_view username) {
	for(auto it{ logins.begin() }; it != logins.end(); ++it) {
		if((*it)[1] == webAddress && (*it)[2] == username) {
			logins.erase(it);
			return true;
		}
	}
	return false;
}

void clearPasswordGeneratorHistory(std::vector <std::string>& passwordGeneratorHistory) {
	passwordGeneratorHistory.resize(0);
}

bool deleteSecureNoteIfSecureNoteExists(std::vector <std::vector <std::string>>& secureNotes, const std::string_view title) {
	for(auto it{ secureNotes.begin() }; it != secureNotes.end(); ++it) {
		if((*it)[1] == title) {
			secureNotes.erase(it);
			return true;
		}
	}
	return false;
}

bool deleteFolderIfFolderExists(std::vector <std::string>& folders, const std::string_view deleteFolderName) {
	for(auto it{ folders.begin() }; it != folders.end(); ++it) {
		if(*it == deleteFolderName) {
			folders.erase(it);
			return true;
		}
	}
	return false;
}
