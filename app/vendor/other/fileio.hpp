#include <string>

std::string readFile(const std::string& filePath);
std::string getFilePath(const std::string& relative);
std::string getAssetPath(const std::string& relative);
std::string getAssetPath(std::initializer_list<std::string> paths);