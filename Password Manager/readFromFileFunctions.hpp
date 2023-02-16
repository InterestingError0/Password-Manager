#pragma once
#include <string>
#include <vector>
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

void readFoldersIntoVector(std::vector <std::string>& folders, const fs::path& foldersPath);
void readLoginsIntoVector(std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath);
void readPasswordGeneratorHistoryIntoVector(std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath);
void readSecureNotesIntoVector(std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath);
