#pragma once

#include <filesystem>
#include <array>

namespace fs = std::filesystem;

const fs::path masterPasswordPath{ "masterPassword.txt" };
const fs::path foldersPath{ "folders.txt" };
const fs::path loginsPath{ "logins.txt" };
const fs::path passwordGeneratorHistoryPath{ "passwordGeneratorHistory.txt" };
const fs::path secureNotesPath{ "secureNotes.txt" };

constexpr std::array <char, 32> specialCharacters{ '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~' };
constexpr std::array <char, 10> numbers{ '0', '1', '2' ,'3', '4', '5', '6', '7', '8', '9' };
constexpr std::array <char, 26> uppercaseLetters{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
constexpr std::array <char, 26> lowercaseLetters{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };