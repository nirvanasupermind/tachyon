#include <string>

#include "values.h"

namespace volcano {

std::string Number::str() const {
    return std::to_string(value);
}

} // namespace volcano