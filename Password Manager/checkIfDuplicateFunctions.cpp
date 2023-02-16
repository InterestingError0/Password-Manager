#include "checkIfDuplicateFunctions.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <cstddef>

bool checkIfLoginIsDuplicate(const std::vector <std::array <std::string, 4>>& logins, const std::string_view webAddress, const std::string_view username) {
	for(std::size_t i{ 0 }; i < logins.size(); i++) {
		if(logins[i][1] == webAddress && logins[i][2] == username) {
			return true;
		}
	}
	return false;
}

bool checkIfSecureNoteIsDuplicate(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string_view title) {
	for(std::size_t i{ 0 }; i < secureNotes.size(); i++) {
		if(secureNotes[i][1] == title) {
			return true;
		}
	}
	return false;
}

bool checkIfFolderIsDuplicate(const std::vector <std::string>& folders, const std::string_view folder) {
	if(std::ranges::find(folders, folder) != folders.end()) {
		return true;
	}
	return false;
}
