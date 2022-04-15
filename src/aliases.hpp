#ifndef ALIASES_HPP
#define ALIASES_HPP

#include <memory>

namespace eris 
{
    template <class T>
    using sh_ptr = std::shared_ptr<T>;
}

#endif