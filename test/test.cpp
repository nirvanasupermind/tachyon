
#include <iostream>
#include <cstring>
#include <cstdint>
#include <chrono>

namespace tachyon_internal {
    uint64_t to_int(int32_t x) {
        uint64_t result = 0;
        std::memcpy(&result, &x, sizeof(x));
        result = (result << 2) + 1;
        return result;
    }

    int32_t from_int(uint64_t x) {
        uint64_t y = x >> 2;
        int32_t result = 0;
        std::memcpy(&result, &y, sizeof(result));
        return result;
    }

    uint64_t to_float(float x) {
        uint64_t result = 0;
        std::memcpy(&result, &x, sizeof(x));
        result = (result + 1) << 2;
        return result;
    }

    float from_float(uint64_t x) {
        uint64_t y = (x - 1) >> 2;
        float result = 0.0f;
        std::memcpy(&result, &y, sizeof(result));
        return result;
    }

    uint64_t minus(uint64_t x) {
        if ((x & 3) == 1) {
            return to_int(-from_int(x));
        }
        else if ((x & 3) == 2) {
            return to_float(-from_float(x));
        }
    }

    uint64_t not_(uint64_t x) {
        return to_int(~from_int(x));
    }

    uint64_t add(uint64_t x, uint64_t y) {
        if ((x & 3) == 1 && (y & 3) == 1) {
            return to_int(from_int(x) + from_int(y));
        }
        else if ((x & 3) == 1 && (y & 3) == 2) {
            return to_float(from_int(x) + from_float(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 1) {
            return to_float(from_float(x) + from_int(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 2) {
            return to_float(from_float(x) + from_float(y));
        }
    }


    uint64_t sub(uint64_t x, uint64_t y) {
        if ((x & 3) == 1 && (y & 3) == 1) {
            return to_int(from_int(x) - from_int(y));
        }
        else if ((x & 3) == 1 && (y & 3) == 2) {
            return to_float(from_int(x) - from_float(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 1) {
            return to_float(from_float(x) - from_int(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 2) {
            return to_float(from_float(x) - from_float(y));
        }
    }

    uint64_t mul(uint64_t x, uint64_t y) {
        if ((x & 3) == 1 && (y & 3) == 1) {
            return to_int(from_int(x) * from_int(y));
        }
        else if ((x & 3) == 1 && (y & 3) == 2) {
            return to_float(from_int(x) * from_float(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 1) {
            return to_float(from_float(x) * from_int(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 2) {
            return to_float(from_float(x) * from_float(y));
        }
    }

    uint64_t div(uint64_t x, uint64_t y) {
        if ((x & 3) == 1 && (y & 3) == 1) {
            return to_int(from_int(x) / from_int(y));
        }
        else if ((x & 3) == 1 && (y & 3) == 2) {
            return to_float(from_int(x) / from_float(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 1) {
            return to_float(from_float(x) / from_int(y));
        }
        else if ((x & 3) == 2 && (y & 3) == 2) {
            return to_float(from_float(x) / from_float(y));
        }
    }

    uint64_t mod(uint64_t x, uint64_t y) {
        if ((x & 3) == 1 && (y & 3) == 1) {
            return to_int(from_int(x) % from_int(y));
        }
        else if ((x & 3) == 1 && (y & 3) == 2) {
            return to_float(std::fmod(from_int(x), from_float(y)));
        }
        else if ((x & 3) == 2 && (y & 3) == 1) {
            return to_float(std::fmod(from_float(x), from_int(y)));
        }
        else if ((x & 3) == 2 && (y & 3) == 2) {
            return to_float(std::fmod(from_float(x), from_float(y)));
        }
    }

    uint64_t and_(uint64_t x, uint64_t y) {
        return to_int(from_int(x) & from_int(y));
    }

    uint64_t or_(uint64_t x, uint64_t y) {
        return to_int(from_int(x) | from_int(y));
    }

    uint64_t xor_(uint64_t x, uint64_t y) {
        return to_int(from_int(x) ^ from_int(y));
    }

    uint64_t lsh(uint64_t x, uint64_t y) {
        return to_int(from_int(x) << from_int(y));
    }

    uint64_t rsh(uint64_t x, uint64_t y) {
        return to_int(from_int(x) >> from_int(y));
    }

    uint64_t lt(uint64_t x, uint64_t y) {
        return (from_int(x) < from_int(y)) ? 11ULL : 3ULL;
    }

    uint64_t le(uint64_t x, uint64_t y) {
        return (from_int(x) <= from_int(y)) ? 11ULL : 3ULL;
    }

    uint64_t gt(uint64_t x, uint64_t y) {
        return (from_int(x) > from_int(y)) ? 11ULL : 3ULL;
    }

    uint64_t ge(uint64_t x, uint64_t y) {
        return (from_int(x) >= from_int(y)) ? 11ULL : 3ULL;
    }
}



int main() {
    auto start = std::chrono::system_clock::now();
    for (uint64_t i = 1ULL;(tachyon_internal::lt(i, 4000000001ULL)) != 3;i = tachyon_internal::add(i, 5ULL)) {
        uint64_t x = tachyon_internal::add(4294967297ULL, 4294967297ULL);

    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}