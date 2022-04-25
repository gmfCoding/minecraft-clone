#include <string>
#include <fstream>
#include <iostream>
#include "AtulPwd.hpp"

std::string readFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "File does not exist:" << filePath << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

std::string getRelativeFilePath(const std::string& relative)
{
    return AtulPWD::mergePaths(AtulPWD::getExecutableDir(), relative);
}

std::string getAssetPath(const std::string& relative)
{
    return AtulPWD::mergePaths(AtulPWD::mergePaths(AtulPWD::getExecutableDir(), "resources"), relative);
}

std::string getAssetPathMany(std::initializer_list<std::string> paths)
{
    std::string current;
    for (auto &&i : paths)
    {
        current = AtulPWD::mergePaths(current, i);
    }
        
    return AtulPWD::mergePaths(AtulPWD::mergePaths(AtulPWD::getExecutableDir(), "resources"), current);
}
