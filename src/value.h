#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>

#include "object.h"

namespace eris {
    enum class ValueType {
        NULL_,
        NUMBER,
        BOOL,
        OBJECT
    };

    class Value {
    public:
        ValueType type;

        union {
            double number;
            bool boolean;
            Object* object = nullptr;
        };

        Value() = default;
        Value(ValueType type);
        Value(ValueType type, double number);
        Value(ValueType type, bool boolean);
        Value(ValueType type, Object* object);
        std::string str() const;
    };
} // namespace eris

#endif // VALUE_H