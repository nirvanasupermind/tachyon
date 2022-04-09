#include <iostream>
#include <string>
#include <fstream>

#include "tokens.cpp"
#include "lexer.cpp"
#include "nodes.cpp"
#include "parser.cpp"
#include "values.cpp"
#include "interpreter.cpp"

std::string read_file(std::string filename)
{
    std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void run(const std::string &text)
{

    volcano::Lexer lexer(text);
    std::vector<volcano::Token> tokens = lexer.generate_tokens();

    // volcano::print_tokens(tokens);

    volcano::Parser parser(tokens);
    std::unique_ptr<volcano::Node> tree = parser.parse();

    volcano::Interpreter interpreter;
    volcano::Number value = interpreter.visit(tree);

    std::cout << value.str() << '\n';
}
int main(int argc, char **argv)
{
    std::string text;

    if (argc > 1)
    {
        std::string filename(argv[1]);
        run(read_file(filename));
    }
    else
    {

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
                std::cerr << e << '\n';
            }
        }
    }
}