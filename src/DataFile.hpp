#pragma once

#include <unordered_map>
#include <string>

class DataFile
{
public:
    DataFile();
    ~DataFile();

    bool loadFromFile(const std::string& filename);

private:
    std::unordered_map<std::string, std::string> mData;
};