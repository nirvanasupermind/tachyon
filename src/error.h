#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace eris {
    void report_error(const std::string &filename, size_t line, const std::string &msg);
} // namespace eris

#endif // ERROR_H