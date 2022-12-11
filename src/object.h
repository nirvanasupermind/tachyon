#ifndef OBJECT_H
#define OBJECT_H

#include <string>

namespace eris {
    enum class ObjectType {
        STRING
    };

    class Object {
    public:
        virtual ObjectType type() const = 0;
        virtual std::string str() const = 0;
    };

    class StringObject : public Object {
    public:
        std::string val;
        StringObject(const std::string& val);
        ObjectType type() const;
        std::string str() const;
    };
} // namespace eris


#endif // OBJECT_H