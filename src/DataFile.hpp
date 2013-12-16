#pragma once

#include <unordered_map>
#include <string>

class DataFile
{
public:
    DataFile();
    ~DataFile();

    bool loadFromFile(const std::string& filename);

    std::string operator[](const std::string& key) const;
    bool hasEntry(const std::string& name) const;

private:
    std::unordered_map<std::string, std::string> mData;
};