#include "printFunctions.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstddef>

void printAllLogins(const std::vector <std::array <std::string, 4>>& logins) {
	std::cout << "Here are the logins stored in your vault:\n\n";
	for(std::size_t i{ 0 }; i < logins.size(); i++) {
		std::cout << "Folder: " << logins[i][0] << "\nWeb Address: " << logins[i][1] << "\nUsername: " << logins[i][2] << "\nPassword: " << logins[i][3] << "\n\n";
	}
}

bool printLoginsInFolder(const std::vector <std::array <std::string, 4>>& logins, const std::string_view folder) {
	bool noLoginsInFolder{ true };
	for(std::size_t i{ 0 }; i < logins.size(); i++) {
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
	for(std::size_t i{ 0 }; i < secureNotes.size(); i++) {
		std::cout << "Folder: " << secureNotes[i][0] << "\nTitle: " << secureNotes[i][1] << "\nContent: " << secureNotes[i][2] << "\n\n";
	}
}

bool printSecureNotesInFolder(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string_view folder) {
	bool noSecureNotesInFolder{ true };
	for(std::size_t i{ 0 }; i < secureNotes.size(); i++) {
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
