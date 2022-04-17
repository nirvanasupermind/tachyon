#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <string>
#include <vector>

#include "parser.hpp"
#include "interpreter.hpp"
#include "aliases.hpp"

void run(const std::string &text)
{
    eris::Parser parser;
    eris::Interpreter interpreter;

    std::cout << interpreter.eval(parser.parse(text), interpreter.global)->str() << '\n';
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::string text;

        while (true)
        {
            try
            {
                std::cout << "> " << std::flush;
                std::getline(std::cin, text);

                if (std::cin.bad())
                {
                    std::cerr << "IO error\n";
                    break;
                }
                else if (std::cin.eof())
                {
                    break;
                }

                run(text);
            }
            catch (const std::string &e)
            {
                std::cerr << "stdin:" << e << '\n';
            }
        }
    }
    else
    {
        std::string filename(argv[1]);
        std::ifstream inFile;

        inFile.open(filename);

        std::stringstream strStream;
        strStream << inFile.rdbuf();

        std::string text = strStream.str();

        try
        {
            run(text);
        }
        catch (std::string e)
        {
            std::cerr << filename << ":" << e << '\n';
            return 1;
        }
    }

    return 0;
}
