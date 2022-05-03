#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <string>
#include <vector>
#include <cctype>

#include "parser.hpp"
#include "interpreter.hpp"
#include "aliases.hpp"

eris::sh_ptr<eris::Environment> global(new eris::Environment());

void run(const std::string &filename, const std::string &text, bool shellMode)
{
    eris::Parser parser;
    eris::Interpreter interpreter;

    try
    {
        eris::sh_ptr<eris::Value> result = interpreter.eval(parser.parse(text), global);

        if(shellMode && result) {
            std::cout << result->str() << '\n';
        }
    }
    catch (const std::string &e)
    {
        std::cerr << filename << ":" << e << '\n';
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Welcome to Eris v0.0." << '\n';
        std::cout << "Use Ctrl+D (i.e. EOF) to exit." << '\n';
        std::string text;

        while (true)
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

            if(text != "") 
            {
                run("stdin", text, true);
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

        if(text == "") {
            std::cerr << "File \"" + text + "\" is empty or does not exist" << '\n'; 
            return 0;
        }

        run(filename, text, false);
    }

    return 0;
}
