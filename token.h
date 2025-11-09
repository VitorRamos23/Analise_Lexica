#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    //Continuar
    //Nomes e atributos dos tokens da linguagem
    CLASS, EXTENDS, INT, STRING, BREAK, PRINT, READ, RETURN, SUPER, IF, ELSE, FOR, NEW, CONSTRUCTOR,
    ID,
    INTEGER_LITERAL,
    OP_LT, OP_GT, OP_LE, OP_GE, OP_PLUS, OP_MINUS, OP_STAR, OP_SLASH, OP_PERCENT, OP_ASSIGN, OP_EQ, OP_NE,
    SEP_LPAREN, SEP_RPAREN, SEP_LBRACKET, SEP_RBRACKET, SEP_LBRACE, SEP_RBRACE, SEP_SEMICOLON, SEP_DOT, SEP_COMMA,
    STRING_LITERAL,
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
