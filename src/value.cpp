#include <string>

#include "object.h"
#include "value.h"

namespace eris {
    Value::Value(ValueType type)
        : type(type) {
    }

    Value::Value(ValueType type, double number)
        : type(type), number(number) {
    }

    Value::Value(ValueType type, bool boolean)
        : type(type), boolean(boolean) {
    }

    Value::Value(ValueType type, Object *object)
        : type(type), object(object) {
    }

    std::string Value::str() const {
        switch (type) {
        case ValueType::NULL_:
            return "null";
        case ValueType::NUMBER:
            return std::to_string(number);
        case ValueType::BOOL:
            return boolean ? "true" : "false";
        case ValueType::OBJECT:
            return object->str();
        default:
            return "";
        }
    }
} // namespace eris