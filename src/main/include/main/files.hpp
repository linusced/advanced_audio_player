#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

#include <CoreServices/CoreServices.h>

std::string getAssetsFilePath();
std::string getApplicationSupportFilePath(const std::string &appName);

std::map<std::string, std::string> getDefaultFilePaths();