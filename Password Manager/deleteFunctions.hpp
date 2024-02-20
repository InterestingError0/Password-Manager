#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <array>

bool deleteLoginIfLoginExists(std::vector <std::vector <std::string>>& logins, std::string_view webAddress, std::string_view username);
void clearPasswordGeneratorHistory(std::vector <std::string>& passwordGeneratorHistory);
bool deleteSecureNoteIfSecureNoteExists(std::vector <std::vector <std::string>>& secureNotes, std::string_view title);
bool deleteFolderIfFolderExists(std::vector <std::string>& folders, std::string_view deleteFolderName);
