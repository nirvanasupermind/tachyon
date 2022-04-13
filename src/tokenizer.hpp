#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <cctype>

#include "token.hpp"
#include "util.hpp"

namespace eris
{
    class Tokenizer
    {
    public:
        std::string string;
        int cursor;

        Tokenizer() = default;

        /**
         * Initializes the string.
         */

        void init(const std::string &string)
        {
            this->string = string;
            this->cursor = 0;
        }

        /**
         * Whether we still have more tokens.
         */
        bool hasMoreTokens() const
        {
            return this->cursor < this->string.size();
        }

        /**
         * Obtains next token.
         */
        Token getNextToken()
        {
            if (!hasMoreTokens())
            {
                return Token("EOF", "<EOF>");
            }

            std::string string = this->string.substr(this->cursor);

            // // Whitespace:
            // if (string[0] == '\n' || isspace(string[0]))
            // {
            //     this->cursor++;


            //     return getNextToken();
            // }
            
            // Numbers:
            if (isdigit(string[0]))
            {
                std::string number;

                while(isdigit(string[this->cursor]))
                {
                    number += string[this->cursor++];
                }

                // std::cout << string[this->cursor] << '\n';

                return Token("NUMBER", number);
            }

            // String:
            if (string[0] == '"')
            {
                std::string s;

                do {
                    s += string[this->cursor++];
                } while(string[this->cursor] != '"' && this->hasMoreTokens());

                s += string[this->cursor++]; // skip "

                return Token("STRING", s);
            }
        }
    };
}

#endif