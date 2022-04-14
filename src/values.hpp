#ifndef VALUES_HPP
#define VALUES_HPP

namespace eris
{
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual std::string str() const = 0;
    };

    class Number : public Value
    {
    public:
        double value;

        Number(double value) : value(value) {}

        std::string str() const
        {
            return std::to_string(value);
        }
    };

    class String : public Value
    {
    public:
        std::string string;

        String(const std::string &string) : string(string) {}

        std::string str() const
        {
            return string;
        }
    };
}

#endif