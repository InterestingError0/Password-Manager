#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

extern const std::array <char, 32> specialCharacters;
extern const std::array <char, 10> numbers;
extern const std::array <char, 26> uppercaseLetters;
extern const std::array <char, 26> lowercaseLetters;

void clearInputBuffer();
std::string getMasterPasswordFromUser();
bool checkIfPasswordMeetsRequirements(std::string_view masterPassword);
void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt, const fs::path& masterPasswordPath);
int menuChoice(int lowerBound, int upperBound);
bool checkIfEnteredMasterPasswordIsValid(const std::string& tempMasterPassword, const fs::path& masterPasswordPath);
std::string getFolderNameFromUser(const std::vector <std::string>& folders);
char generateCharacter();
