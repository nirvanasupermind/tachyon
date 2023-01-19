#include <string>

class Val {
};

class Int: public Val {
public:
    int val;

    Int(int val)
        : val(val) {
    }

    operator int() const {
        return val;
    }
};

class Double: public Val {
public:
    double val;

    Double(double val)
        : val(val) {
    }

    operator double() const {
        return val;
    }
};

class Bool: public Val {
public:
    bool val;

    Bool(bool val)
        : val(val) {
    }

    operator bool() const {
        return val;
    }
};

class Object: public Val {
public:
    std::map<std::string, Object> props{};

    Object() = default;
};