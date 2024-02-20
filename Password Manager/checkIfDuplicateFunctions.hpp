#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

bool checkIfLoginIsDuplicate(const std::vector <std::vector <std::string>>& logins, std::string_view webAddress, std::string_view username);
bool checkIfSecureNoteIsDuplicate(const std::vector <std::vector <std::string>>& secureNotes, std::string_view title);
bool checkIfFolderIsDuplicate(const std::vector <std::string>& folders, std::string_view folder);
