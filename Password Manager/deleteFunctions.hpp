#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

bool deleteLoginIfLoginExists(std::vector <std::array <std::string, 4>>& logins, const std::string_view webAddress, const std::string_view username);
void deletePasswordGeneratorHistory(std::vector <std::string>& passwordGeneratorHistory);
bool deleteSecureNoteIfSecureNoteExists(std::vector <std::array <std::string, 3>>& secureNotes, const std::string_view title);
bool deleteFolderIfFolderExists(std::vector <std::string>& folders, const std::string_view deleteFolderName);
