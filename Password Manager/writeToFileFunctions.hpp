#pragma once
#include <string>
#include <vector>
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

void writeFoldersToFile(const std::vector <std::string>& folders, const fs::path& foldersPath);
void writeLoginsToFile(const std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath);
void writePasswordGeneratorHistoryToFile(const std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath);
void writeSecureNotesToFile(const std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath);
