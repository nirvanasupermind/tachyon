#ifndef VALUES_H
#define VALUES_H

#include <string>

namespace volcano {

class Number {
public:
    double value;

    Number(double value) : value(value) {}

    std::string str() const;
};

} // namespace volcano


#endif // VALUES_H