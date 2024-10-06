#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "token.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "transpiler.h"

void transpile(const std::string& filename, const std::string& text, bool i) {
    tachyon::Lexer lexer(filename, text);
    std::vector<tachyon::Token> tokens = lexer.make_tokens();
    tachyon::Parser parser(filename, tokens);
    std::shared_ptr<tachyon::Node> tree = parser.parse();
    tachyon::Transpiler transpiler;
    transpiler.visit(tree);
    std::string stl = R"V0G0N( 
    #include <iostream>
    #include <cstring>
    #include <cstdint>

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
            if((x & 3) == 1) {
                return to_int(-from_int(x));
            } else if((x & 3) == 2) {
                return to_float(-from_float(x));
            }
        }

        uint64_t not_(uint64_t x) {
            return to_int(~from_int(x));
        }

        uint64_t add(uint64_t x, uint64_t y) {
            if((x & 3) == 1 && (y & 3) == 1) {
                return to_int(from_int(x) + from_int(y));
            } else if((x & 3) == 1 && (y & 3) == 2) {
                return to_float(from_int(x) + from_float(y));
            } else if((x & 3) == 2 && (y & 3) == 1) {
                return to_float(from_float(x) + from_int(y));
            } else if((x & 3) == 2 && (y & 3) == 2) {
                return to_float(from_float(x) + from_float(y));
            }
        }


        uint64_t sub(uint64_t x, uint64_t y) {
            if((x & 3) == 1 && (y & 3) == 1) {
                return to_int(from_int(x) - from_int(y));
            } else if((x & 3) == 1 && (y & 3) == 2) {
                return to_float(from_int(x) - from_float(y));
            } else if((x & 3) == 2 && (y & 3) == 1) {
                return to_float(from_float(x) - from_int(y));
            } else if((x & 3) == 2 && (y & 3) == 2) {
                return to_float(from_float(x) - from_float(y));
            }
        }

        uint64_t mul(uint64_t x, uint64_t y) {
            if((x & 3) == 1 && (y & 3) == 1) {
                return to_int(from_int(x) * from_int(y));
            } else if((x & 3) == 1 && (y & 3) == 2) {
                return to_float(from_int(x) * from_float(y));
            } else if((x & 3) == 2 && (y & 3) == 1) {
                return to_float(from_float(x) * from_int(y));
            } else if((x & 3) == 2 && (y & 3) == 2) {
                return to_float(from_float(x) * from_float(y));
            }
        }

        uint64_t div(uint64_t x, uint64_t y) {
            if((x & 3) == 1 && (y & 3) == 1) {
                return to_int(from_int(x) / from_int(y));
            } else if((x & 3) == 1 && (y & 3) == 2) {
                return to_float(from_int(x) / from_float(y));
            } else if((x & 3) == 2 && (y & 3) == 1) {
                return to_float(from_float(x) / from_int(y));
            } else if((x & 3) == 2 && (y & 3) == 2) {
                return to_float(from_float(x) / from_float(y));
            }
        }

        uint64_t mod(uint64_t x, uint64_t y) {
            if((x & 3) == 1 && (y & 3) == 1) {
                return to_int(from_int(x) % from_int(y));
            } else if((x & 3) == 1 && (y & 3) == 2) {
                return to_float(std::fmod(from_int(x), from_float(y)));
            } else if((x & 3) == 2 && (y & 3) == 1) {
                return to_float(std::fmod(from_float(x), from_int(y)));
            } else if((x & 3) == 2 && (y & 3) == 2) {
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

    void setup() {
    }


    )V0G0N";

    std::string filename_noext = filename.substr(0, filename.size() - 8);
    std::ofstream out_file;
    out_file.open(filename_noext + ".cpp");
    out_file << stl << "int main(){\nsetup();\n" << transpiler.code.str() << "\nreturn 0;\n}";
    out_file.close();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    // Windows
    system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + ".exe-std=c++11").c_str());
    if (!i) {
        system(("del " + filename_noext + ".cpp").c_str());
    }
#else
    // Linux and Mac
    system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + " -std=c++11").c_str());
    if (!i) {
        system(("rm -rf " + filename_noext + ".cpp").c_str());
    }
#endif
    // std::size_t idx = filename.find_last_of("/\\");
}

int main(int argc, char** argv) {
    if ((argc < 2)) {
        std::cerr << "Usage: tachyonc [file]" << '\n';
        std::cerr << "Options (must be added after filename):" << '\n';
        std::cerr << "-i: Keep intermediate C++ file" << '\n';
        return 1;
    }
    else {
        std::string filename(argv[1]);
        std::ifstream in_file;

        in_file.open(filename);

        std::stringstream strStream;
        strStream << in_file.rdbuf();

        std::string text = strStream.str();

        if (text == "") {
            std::cerr << "File \"" + filename + "\" is empty or does not exist" << '\n';
            return 0;
        }

        try {
            if (argc == 3 && std::string(argv[2]) == "-i") {
                transpile(filename, text, true);
            }
            else {
                transpile(filename, text, false);
            }
            in_file.close();
        }
        catch (const std::string& e) {
            std::cerr << e << '\n';
            return 1;
        }
    }

    return 0;
}