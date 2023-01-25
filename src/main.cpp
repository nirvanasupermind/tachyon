#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include "token.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "transpiler.h"

void transpile(const std::string& in_filename, const std::string& out_filename, const std::string& text) {
    eris::Lexer lexer(text, in_filename);
    std::vector<eris::Token> tokens = lexer.generate_tokens();
    eris::Parser parser(tokens, in_filename);
    std::shared_ptr<eris::Node> tree = parser.parse();
    eris::Transpiler transpiler(in_filename);
    std::ofstream out_file;
    out_file.open(out_filename);
    out_file << transpiler.generate_code(tree.get());
    out_file.close();
}

int main(int argc, char** argv) {
    if ((argc < 4)) {
        std::cerr << "Usage: erisc [infile] -o [outfile]" << '\n';
        return 1;
    }
    else {
        std::string in_filename(argv[1]);
        std::ifstream in_file;

        in_file.open(in_filename);

        std::stringstream strStream;
        strStream << in_file.rdbuf();

        std::string text = strStream.str();

        if (text == "") {
            std::cerr << "File \"" + in_filename + "\" is empty or does not exist" << '\n';
            return 0;
        }

        std::string out_filename(argv[3]);

        try {
            transpile(in_filename, out_filename, text);
            in_file.close();
        }
        catch (const std::string& e) {
            std::cerr << e << '\n';
            return 1;
        }
    }

    return 0;
}