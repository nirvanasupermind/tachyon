#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

namespace eris
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1.0);
}

#endif