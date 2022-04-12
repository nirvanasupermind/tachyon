#ifndef VALUES_H
#define VALUES_H

#include <string>
#include <cstdint>

namespace eris
{
    enum ValueKind
    {
        IntVal,
        DoubleVal
    };

    class Value
    {
    public:
        virtual ValueKind kind() const = 0;
        virtual std::string str() const = 0;
    };

    class Number: public Value 
    {
    public:
        virtual double getval() const = 0;
    };

    class Int : public Number
    {
    public:
        std::int32_t value;

        Int(std::int32_t value): value(value) {};
        
        ValueKind kind() const;
        double getval() const;
        std::string str() const;
    };

    class Double : public Number
    {
    public:
        double value;

        Double(double value): value(value) {};
        
        ValueKind kind() const;
        double getval() const;
        std::string str() const;
    };
} // namespace eris

#endif // VALUES_H