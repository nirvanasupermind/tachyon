#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <string>
#include <vector>

#include "token.hpp"
#include "tokenizer.hpp"

void run(std::string filename, std::string text)
{
    eris::Tokenizer tokenizer;
    
    tokenizer.init(text);

    while (tokenizer.hasMoreTokens())
    {
        eris::Token token = tokenizer.getNextToken();
        std::cout << token.str() << '\n';
    }
 }

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: punylisp [script]" << '\n';
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
            run(filename, text);
        }
        catch (std::string e)
        {
            std::cerr << e << '\n';
            return 1;
        }
    }

    return 0;
}
