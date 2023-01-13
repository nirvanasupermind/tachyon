#ifndef ENV_H
#define ENV_H

#include <string>
#include <set>
#include <memory>

namespace eris {
    class Env {
    private:
        std::set<std::string> scope{};
        std::shared_ptr<Env> parent{};
    public:
        Env();
        Env(std::shared_ptr<Env> parent);
        void check_name(const std::string& name, const std::string& filename, size_t line);
        void add_name(const std::string& name);
    };
} // namespace eris

#endif // ENV_H