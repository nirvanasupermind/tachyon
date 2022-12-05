#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace eris {
    // Throws an Eris error as an std::string
    void raise_error(const std::string &filename, size_t line, const std::string &msg);
} // namespace eris

#endif // ERROR_H