#pragma once

#include <string>
namespace AtulPWD {
  std::string getExecutablePath();
  std::string getExecutableDir();
  std::string mergePaths(std::string pathA, std::string pathB);
  bool checkIfFileExists (const std::string& filePath);
}