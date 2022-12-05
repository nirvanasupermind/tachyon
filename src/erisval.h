#ifndef ERISVAL_H
#define ERISVAL_H

#include <string>

namespace eris {
    // Eris value type
    enum class ErisValType {
        NULL_,
        NUMBER,
        BOOL
    };

    // Eris value (tagged union)
    class ErisVal {
    public:
        ErisValType type;
        union {
            double number;
            bool boolean;
        };

        ErisVal(ErisValType type);
        ErisVal(ErisValType type, double number);
        ErisVal(ErisValType type, bool boolean);
        std::string str() const;
    };
} // namespace eris

#endif // ERISVAL_H