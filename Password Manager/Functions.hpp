#pragma once
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

void clearInputBuffer();
bool checkIfPasswordMeetsRequirements(const std::string& masterPassword);
std::pair <std::string, std::string> hash(std::string& masterPassword, const std::string& salt = "");
void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt, const fs::path& masterPasswordPath);
int menuChoice(int lowerBound, int upperBound);
bool checkIfEnteredMasterPasswordIsValid(std::string& tempMasterPassword, const fs::path& masterPasswordPath);
std::string getFolderNameFromUser(const std::vector <std::string>& folders);
void readFoldersIntoVector(std::vector <std::string>& folders, const fs::path& foldersPath);
void readLoginsIntoVector(std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath);
void readPasswordGeneratorHistoryIntoVector(std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath);
void readSecureNotesIntoVector(std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath);
void writeFoldersToFile(const std::vector <std::string>& folders, const fs::path& foldersPath);
void writeLoginsToFile(const std::vector <std::array <std::string, 4>>& logins, const fs::path& loginsPath);
void writePasswordGeneratorHistoryToFile(const std::vector <std::string>& passwordGeneratorHistory, const fs::path& passwordGeneratorHistoryPath);
void writeSecureNotesToFile(const std::vector <std::array <std::string, 3>>& secureNotes, const fs::path& secureNotesPath);
bool deleteLoginIfLoginExists(std::vector <std::array <std::string, 4>>& logins, const std::string& webAddress, const std::string& username);
void deletePasswordGeneratorHistory(std::vector <std::string>& passwordGeneratorHistory);
bool deleteSecureNoteIfSecureNoteExists(std::vector <std::array <std::string, 3>>& secureNotes, const std::string& title);
void deleteFolder(std::vector <std::string>& folders, const std::string& deleteFolderName);
bool checkIfLoginIsDuplicate(const std::vector <std::array <std::string, 4>>& logins, const std::string& webAddress, const std::string& username);
bool checkIfSecureNoteIsDuplicate(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string& title);
bool checkIfFolderIsDuplicate(const std::vector <std::string>& folders, const std::string& folder);
void printAllLogins(const std::vector <std::array <std::string, 4>>& logins);
bool printLoginsInFolder(const std::vector <std::array <std::string, 4>>& logins, const std::string& folder);
void printPasswordGeneratorHistory(const std::vector <std::string>& passwordGeneratorHistory);
void printAllSecureNotes(const std::vector <std::array <std::string, 3>>& secureNotes);
bool printSecureNotesInFolder(const std::vector <std::array <std::string, 3>>& secureNotes, const std::string& folder);
void printFolders(const std::vector <std::string>& folders);
char generateCharacter();
