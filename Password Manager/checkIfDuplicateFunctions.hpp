#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

bool checkIfLoginIsDuplicate(const std::vector <std::array <std::string, 4>>& logins, const std::string_view webAddress, const std::string_view username);
bool checkIfSecureNoteIsDuplicate(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string_view title);
bool checkIfFolderIsDuplicate(const std::vector <std::string>& folders, const std::string_view folder);
