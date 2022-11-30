#include <string>

#include "value.h"

namespace eris {

    ErisValue::ErisValue(ErisValueType type)
        : type(type) {
    }

    ErisValue::ErisValue(ErisValueType type, double number)
        : type(type), number(number) {
    }

    std::string ErisValue::str() const {
        switch (type) {
        case ErisValueType::NULL_:
            return "null";
        case ErisValueType::NUMBER:
            return std::to_string(number);
        default:
            return "";
        }
    }
} // namespace eris