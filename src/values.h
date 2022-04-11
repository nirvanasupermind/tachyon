#ifndef VALUES_H
#define VALUES_H

#include <string>

namespace eris
{
    class Value
    {
        virtual std::string str() const = 0;
    };

    class Number: public Value
    {
    public:
        double value;

        Number(double value) : value(value) {}

        std::string str() const;
    };

} // namespace eris

#endif // VALUES_H