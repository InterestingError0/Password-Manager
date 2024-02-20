#pragma once
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <cryptopp/secblock.h>

namespace fs = std::filesystem;

template <typename T>
std::string vecToStr(const std::vector <T>& vec) {
    std::string output;

    if constexpr(std::is_same_v<T, std::string>) {
        for(const std::string& str : vec) {
            output += str + "\n";
        }
    } else {
        for(const std::vector <std::string>& outerVec : vec) {
            for(const std::string& str : outerVec) {
                output += str + "\n";
            }
        }
    }
    return output;
}

void writeToFile(const std::string& data, const fs::path& filePath);
