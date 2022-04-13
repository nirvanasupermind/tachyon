#ifndef UTIL_HPP
#define UTIL_HPP

namespace eris
{
    std::string trimSpaces(const std::string &s)
    {        
        int i = 0;

        while(s.substr(i).front() == ' ')
        {
            i++;
        }

        return s.substr(i);
    }
}
#endif