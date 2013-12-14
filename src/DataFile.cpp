#include "DataFile.hpp"

#include <fstream>
#include <random>

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
        if (value.find('-') != std::string::npos)
        {
            int pos = value.find('-');
            bool floating = value.find('.') != std::string::npos;

            std::string lower = value.substr(0, pos);
            while(lower.back() == ' ')
                lower.erase(lower.size()-1);

            std::string upper = value.substr(pos+1);
            while(upper.front() == ' ')
                upper.erase(0, 1);

            float lowerF = 0, upperF = 0;
            int found = sscanf((lower + "|" + upper).c_str(), "%f|%f", &lowerF, &upperF);

            if (found == 2)
            {
                char tmp[256];
                if (floating)
                    sprintf(tmp, "§%f-%f", lowerF, upperF);
                else
                    sprintf(tmp, "§%i-%i", (int)lowerF, (int)upperF);

                value = tmp;
            }
        }
    
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

    std::string value = mData.at(key);

    if (value[0] == '§')
    {
        int pos = value.find('-');
        bool floating = value.find('.') != std::string::npos;

        float lower = atof(value.substr(1, pos).c_str());
        float upper = atof(value.substr(pos + 1).c_str());

        std::random_device dev;

        float fval;
        if (floating)
            fval = std::uniform_real_distribution<float>(lower, upper)(dev);
        else
            fval = std::uniform_int_distribution<int>(lower, upper)(dev);

        char tmp[32];
        if (floating)
            sprintf(tmp, "%f", fval);
        else
            sprintf(tmp, "%i", (int)fval);

        value = tmp;
    }

    return value;
}
