#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

void printAllLogins(const std::vector <std::array <std::string, 4>>& logins);
bool printLoginsInFolder(const std::vector <std::array <std::string, 4>>& logins, const std::string_view folder);
void printPasswordGeneratorHistory(const std::vector <std::string>& passwordGeneratorHistory);
void printAllSecureNotes(const std::vector <std::array <std::string, 3>>& secureNotes);
bool printSecureNotesInFolder(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string_view folder);
void printFolders(const std::vector <std::string>& folders);