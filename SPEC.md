**INCOMPLETE**

# 1 - Introduction
Eris is a general-purpose, lightweight, dynamic programming language with class-based object-oriented programming support.

Eris is designed to be simple enough that many programmers can achieve fluency in the language.
Eris in spirit is related to Python & C++ but is organized rather differently, with a number of aspects omitted and a few ideas from other languages included.

## 1.1 - Example Programs
See the `examples` folder to see several example Eris programs.

# 2 - Grammar

This section defines the formal grammar used to define the lexical and syntactic structure of an Eris program.

## 2.1 - Grammar Notation

Both grammars are defined in standard Backus-Naur form, with some slight modifications:

* The following escape sequences are used for control characters throughout the grammar definitions:
    * Horizontal tab (ASCII char code `0x09`) - `\t`
    * Line feed, newline (ASCII char code `0x0A`) - `\n`
    * Vertical tab (ASCII char code `0x0B`) - `\v`
    * Form feed, new page (ASCII char code `0x0C`) - `\f`
    * Carriage return (ASCII char code `0x0D`) - `\r`
    * Double quote - `\"`
* In some rare cases, it may be impractical to list all productions for a given grammar rule, so the rule will be instead be defined using a descriptive English text. For example:
    ```
    <any-char> ::= (any unicode character);
    ```

## 2.2 - The Lexical Grammar

The rules here are *ordered*. Matches are attempted from top to bottom.

The `<skip>` token is ignored in parsing.

```bnf
<any-char> := (any unicode character);
<non-newline-char> := (any unicode character, but not a newline);
<non-double-quote-char> := (any unicode character, but not a double quote);
<whitespace-char> := " " | "\t" | "\n" | "\v" | "\f" | "\r";
<letter> ::= "A" | "B" | "C" | "D" | "E" | "F" | "G"
       | "H" | "I" | "J" | "K" | "L" | "M" | "N"
       | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
       | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
       | "c" | "d" | "e" | "f" | "g" | "h" | "i"
       | "j" | "k" | "l" | "m" | "n" | "o" | "p"
       | "q" | "r" | "s" | "t" | "u" | "v" | "w"
       | "x" | "y" | "z" ;
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
<identifier-start-char> := <letter> | "_";
<identifier-char> := <letter> | <digit> | "_";
<whitespace> ::= <whitespace-char> | <whitespace> <whitespace-char>; 
<single-line-comment> ::= "//" | <single-line-comment> <non-newline-char>;
<multi-line-comment-init> ::= "/*" | <multi-line-comment-init> <any-char>;
<multi-line-comment> ::= <multi-line-comment-init> "*/";
<skip> ::= <whitespace> | <single-line-comment> | <multi-line-comment>;
<double> ::= <int> "." <opt-int>;
<int> ::= <digit> | <int> digit;
<identifier> ::= <identifier-start-char> | <identifier> <identifier-char>;
<equality-operator> ::= "==" | "!=";
<simple-assign> ::= "=";
<complex-assign> ::= "*=" | "-=" | "+=";
<additive-operator> ::= "+" | "-";
<relational-operator> ::= ">" | "<" | ">=" | "<=";
<string-init> ::= "\"" | <string-init> <non-double-quote-char>;
<string> ::= <string-init> "\"";
```

## 2.3 - The Syntactic Grammar
```
<program> ::= <statement-list>;
<statement-list> ::= <statement> | <statement-list> <statement>;
<statement> ::= <expression-statement> | <EmptyStatement> | <BlockStatement> | <VariableStatement> | <IterationStatement> | <ReturnStatement> | <RequireStatement> | <FunctionDeclaration> | <ClassDeclaration> | <NamespaceDeclaration>;
<NamespaceDeclaration> ::= "namespace" <identifier> <BlockStatement>;
<ClassDeclaration> ::= "class" <identifier> <OptClassExtends> <BlockStatement>;
<ClassExtends> ::= "extends" <Expression>;
<FunctionDeclaration> ::= "def" <identifier> "(" <OptFormalParameterList> ")" BlockStatement;
<FormalParameterList> ::= <identifier> | <FormalParameterList> "," <identifier>;
<ReturnStatement> ::= "return" <Expression> ";";
<RequireStatement> ::= "require" <string> ";";
<IterationStatement> ::= <WhileStatement> | <DoWhileStatement> | <ForStatement>;
<WhileStatement> ::= "while" "(" <Expression> ")" <Statement>;
<DoWhileStatement> ::= "do" <Statement> "while" <Expression> ";";
<ForStatement> ::= "for" "(" <ForStatementInit> ";" <Expression> ";" <Expression> ")" 
<ForStatementInit> ::= <VariableStatementInit> | <Expression>;
<IfStatement> ::= "if" "(" <Expression> ")" <Statement> | "if" "(" <Expression> ")" <Statement> "else" <Statement>;
 ```
# 3 - The Language
## 3.1 - Values and Types
## 3.2 - Environments
## 3.3 - Variables
## 3.4 - Expressions
## 3.5 - Statements
## 4 - Standard Library
### 4.1 - Global Functions
### 4.2 - The Class Object
### 4.3 - The Class String
### 4.4 - The Class List
### 4.5 - The Class Function