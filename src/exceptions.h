#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

namespace eris
{
    class Exception: public std::exception
    {
    public:
        std::string message{};

        Exception(const std::string &message): message(message) {}
    };

} // namespace eris

#endif // EXCEPTIONS_H