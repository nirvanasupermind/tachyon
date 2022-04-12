#include <string>
#include <sstream>

#include "values.h"

namespace eris
{
    ValueKind Int::kind() const
    {
        return ValueKind::IntVal;
    }

    double Int::getval() const
    {
        return value;
    }

    std::string Int::str() const
    {
        return std::to_string(value);
    }

    ValueKind Double::kind() const
    {
        return ValueKind::DoubleVal;
    }

    double Double::getval() const
    {
        return value;
    }

    std::string Double::str() const
    {
        return std::to_string(value);
    }

    ValueKind Atom::kind() const
    {
        return ValueKind::AtomVal;
    }

    std::string Atom::str() const
    {
        return atom;
    }
} // namespace eris