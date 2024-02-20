#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

void printAllLogins(const std::vector <std::vector <std::string>>& logins);
bool printLoginsInFolder(const std::vector <std::vector <std::string>>& logins, const std::string_view folder);
void printPasswordGeneratorHistory(const std::vector <std::string>& passwordGeneratorHistory);
void printAllSecureNotes(const std::vector <std::vector <std::string>>& secureNotes);
bool printSecureNotesInFolder(const std::vector <std::vector <std::string>>& secureNotes, const std::string_view folder);
void printFolders(const std::vector <std::string>& folders);