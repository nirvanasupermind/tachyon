#include <string>

#include "erisval.h"

namespace eris {
    ErisVal::ErisVal(ErisValType type)
        : type(type) {
    }

    ErisVal::ErisVal(ErisValType type, double number)
        : type(type), number(number) {
    }

    ErisVal::ErisVal(ErisValType type, bool boolean)
        : type(type), boolean(boolean) {
    }

    std::string ErisVal::str() const {
        switch (type) {
        case ErisValType::NULL_:
            return "null";
        case ErisValType::NUMBER:
            return std::to_string(number);
        case ErisValType::BOOL:
            return boolean ? "true" : "false";
        default:
            return "";
        }
    }
} // namespace eris