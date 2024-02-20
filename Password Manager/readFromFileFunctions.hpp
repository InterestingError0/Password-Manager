#pragma once
#include <string>
#include <vector>
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
void loadToVec(std::vector <T>& vec, const std::string& str, const std::size_t innerVecSize = 0) {
    if constexpr(std::is_same_v<T, std::string>) {
        for(std::size_t i{ 0 }, prevDelimPos{ 0 }; i < str.length(); i++) {
            if(str[i] == '\n') {
                vec.push_back(str.substr(prevDelimPos, i - prevDelimPos));
                prevDelimPos = i + 1;
            }
        }
    } else {
        std::vector <std::string> innerVec;

        for(std::size_t i{ 0 }, prevI{ 0 }; i < str.length(); i++) {
            if(str[i] == '\n') {
                innerVec.push_back(str.substr(prevI, i - prevI));
                prevI = i + 1;
            }
            if(innerVec.size() == innerVecSize) {
                vec.push_back(innerVec);
                innerVec.clear();
            }
        }
    }
}

std::string loadFile(const fs::path& filePath);
