#include "DataFile.hpp"

#include <fstream>

DataFile::DataFile()
{
}

DataFile::~DataFile()
{
}

bool DataFile::loadFromFile(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::in);
    
    auto readUntil = [&ifs](const std::string& chars) -> std::string
    {
        std::string ret;

        char c = 0;
        for(;;)
        {
            c = ifs.get();

            if (chars.find(c) != std::string::npos)
                break;

            if (!ifs || ifs.eof())
                break;

            ret += c;
        }

        return ret;
    };

    auto skipWhitespace = [&ifs]()
    {
        const std::string whitespace = " \n\r\t";

        char c = 0;
        for(;;)
        {
            c = ifs.get();

            if (whitespace.find(c) == std::string::npos)
                break;

            if (!ifs || ifs.eof())
                return;
        }

        ifs.unget();
    };

    if (!ifs || ifs.eof())
        return false;

    do
    {
        skipWhitespace();
        
        std::string key = readUntil(":");

        skipWhitespace();

        std::string value = readUntil("\n\r");
    
        if (!key.empty() && !value.empty())
        {
            mData[key] = value;
        }
        else
            return false;
    } while (ifs && !ifs.eof());

    return true;
}

std::string DataFile::operator[](const std::string& key) const
{
    if (mData.count(key) == 0)
        return "";

    return mData.at(key);
}
