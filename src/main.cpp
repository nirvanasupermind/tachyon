#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <string>
#include <vector>
#include <cctype>

#include "parser.hpp"
#include "interpreter.hpp"
#include "aliases.hpp"
#include "builtins.hpp"

eris::sh_ptr<eris::Environment> global(new eris::Environment());

void launchBuiltins() 
{
    global->define("Object", eris::builtins::Object); 
    global->define("String", eris::builtins::String);
    eris::builtins::String->members->define("len", eris::builtins::Stringlen);
    global->define("print", eris::builtins::print);  
}

void run(const std::string &filename, const std::string &text)
{
    eris::Parser parser;
    eris::Interpreter interpreter(global);

    try
    {
        interpreter.eval(parser.parse(text), global);
    }
    catch (const std::string &e)
    {
        std::cerr << filename << ":" << e << '\n';
    }
}

int main(int argc, char **argv)
{
    launchBuiltins();
    
    if (argc < 2)
    {
        std::cerr << "Usage: eris [script]" << '\n';
        return 0;
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
            std::cerr << "File \"" + filename + "\" is empty or does not exist" << '\n'; 
            return 0;
        }

        run(filename, text);
    }

    return 0;
}
