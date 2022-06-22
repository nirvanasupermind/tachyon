**INCOMPLETE**

# 1 - Introduction
Eris is a general-purpose, lightweight, dynamic language with class-based object-oriented programming support. 

Eris is designed to be simple enough that many programmers can achieve fluency in the language.
Eris in spirit is related to Python & C++ but is organized rather differently, with a number of aspects omitted and a few ideas from other languages included.


## 1.1 - Example Programs
See the `examples` folder to see several example Eris programs and their expected outputs.

# 2 - Grammar

This section defines the formal grammar used to define the lexical and syntactic structure of an Eris program.

## 2.1 - Grammar Notation

Both grammars are defined in standard Backus-Naur form. Definitions may also be shortened using regular expressions in the lexical grammar. For example, the definition:

```bnf
<digit> ::= [0-9];
```

Is a convenient shorthand for:

```bnf
<digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9';
```

## 2.2 - The Lexical Grammar

The productions here are *ordered*. Matches are attempted from top to bottom.

```bnf
<whitespace> ::= \s+; 
<single-line-comment> ::= /.*;
<multi-line-comment> ::= /\*[\s\S]*?\*/;
<skip> ::= <whitespace> | <single-line-comment> | <multi-line-comment>;
<identifier> ::= \w+;
<equality-operator> ::= '=
<simple-assign> ::= '=';
<complex-assign> ::= [*+-]=;
<additive-operator> ::= 
<int> ::= \d+(?![.eE]);
<double> ::= \d+(\.\d*)?([eE][+-]?\d+)?;

```

## 2.3 - The Syntactic Grammar
```
```
# 3 - The Language
## 3.1 - Values and Types
## 3.2 - Environments
## 3.3 - Variables
## 3.4 - Expressions
## 3.5 - Statements
## 4 - Standard Library