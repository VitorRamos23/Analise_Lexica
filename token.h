#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

// Enumeração de todos os nomes de tokens
enum TokenName {
    // Identificadores e Literais
    ID,                 // [cite: 196]
    INTEGER_LITERAL,    // [cite: 198]
    STRING_LITERAL,     // [cite: 201]

    // Operadores [cite: 199]
    OP_LT,              // <
    OP_GT,              // >
    OP_LE,              // <=
    OP_GE,              // >=
    OP_PLUS,            // +
    OP_MINUS,           // -
    OP_STAR,            // *
    OP_SLASH,           // /
    OP_PERCENT,         // %
    OP_ASSIGN,          // =
    OP_EQ,              // ==
    OP_NE,              // !=

    // Separadores [cite: 200]
    SEP_LPAREN,         // (
    SEP_RPAREN,         // )
    SEP_LBRACKET,       // [
    SEP_RBRACKET,       // ]
    SEP_LBRACE,         // {
    SEP_RBRACE,         // }
    SEP_SEMICOLON,      // ;
    SEP_DOT,            // .
    SEP_COMMA,          // ,

    // Fim de Arquivo
    END_OF_FILE
};

// Estrutura para armazenar as informações de um token
class Token {
public:
    int name;
    string lexeme;

    Token(int name, string lexeme) {
        this->name = name;
        this->lexeme = lexeme;
    }

    Token(int name) {
        this->name = name;
        this->lexeme = "";
    }
};

#endif