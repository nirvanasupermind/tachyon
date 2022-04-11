#include <string>

#include "values.h"

namespace eris {

std::string Number::str() const {
    return std::to_string(value);
}

} // namespace eris