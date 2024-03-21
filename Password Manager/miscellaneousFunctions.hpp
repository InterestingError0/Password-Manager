#pragma once
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <filesystem>
#include <utility>

void clearInputBuffer();
std::string getMasterPasswordFromUser();
bool checkIfPasswordMeetsRequirements(std::string_view masterPassword);
void saveMasterPasswordToFile(const std::pair <std::string, std::string>& hashAndSalt);
int menuChoice(int upperBound);
bool checkIfEnteredMasterPasswordIsValid(const std::string& tempMasterPassword);
char generateCharacter();
