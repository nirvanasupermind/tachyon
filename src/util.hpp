#ifndef UTIL_HPP
#define UTIL_HPP

namespace eris
{
    std::string addressString(const void *p)
    {
        std::ostringstream address;
        address << p;
        std::string name = address.str();
        return name;
    }

    int countNewlines(const std::string &s)
    {
        int result = 0;

        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '\n')
                result++;
        }

        return result;
    }
}
#endif