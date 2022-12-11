
#include <string>

#include "object.h"

namespace eris {
    StringObject::StringObject(const std::string& val)
        : val(val) {
    }

    ObjectType StringObject::type() const {
        return ObjectType::STRING;
    }

    std::string StringObject::str() const {
        return val;
    }
} // namespace eris