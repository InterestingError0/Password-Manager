#include "writeToFileFunctions.hpp"
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>

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

void writeSecureNotesToFile(const std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath) {
	std::ofstream oSecureNotes(secureNotesPath);
	for(const auto& x : secureNotes) {
		for(const std::string& y : x) {
			oSecureNotes << y << '\n';
		}
	}
}
