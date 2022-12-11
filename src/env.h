#ifndef ENV_H
#define ENV_H

#include <string>
#include <memory>
#include <map>

#include "value.h"

namespace eris {
    class Env {
    private:
        std::map<std::string, Value> record{};
        Env* parent;
        Env* find_env(const std::string &name);
    public:
        Env();
        Env(Env*  parent);
        Value get(const std::string &name);
        void set(const std::string &name, const Value& val);
        void def(const std::string &name, const Value& val);    
    };
} // namespace eris

#endif // ENV_H