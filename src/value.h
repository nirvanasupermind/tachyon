#ifndef VALUE_H
#define VALUE_H

#include <string>

namespace eris {
    enum class ErisValueType {
        NULL_,
        NUMBER
    };

    class ErisValue {
    public:
        ErisValueType type;
        union {
            double number;
        };

        ErisValue(ErisValueType type);

        ErisValue(ErisValueType type, double number);

        std::string str() const;
    };
} // namespace eris

#endif // VALUE_H