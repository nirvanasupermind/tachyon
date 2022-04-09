#ifndef VALUES_H
#define VALUES_H

#include <string>

namespace eris {

class Number {
public:
    double value;

    Number(double value) : value(value) {}

    std::string str() const;
};

} // namespace eris


#endif // VALUES_H