#ifndef UTIL_HPP
#define UTIL_HPP

namespace eris
{
    int countNewlines(const std::string &s)
    {        
        int result = 0;

        for(int i = 0; i < s.length(); i++)
        {
            if(s[i] == '\n')
                result++;
        }

        return result;
    }
}
#endif