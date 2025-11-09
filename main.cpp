#include "scanner.h"
#include <iostream> // Para cout

// Função auxiliar para converter o nome do token em string
string tokenNameToString(int name)
{
    switch (name)
    {
        case ID: return "ID";
        case INTEGER_LITERAL: return "INTEGER_LITERAL";
        case OP_LT: return "OP_LT";
        case OP_GT: return "OP_GT";
        case OP_LE: return "OP_LE";
        case OP_GE: return "OP_GE";
        case OP_PLUS: return "OP_PLUS";
        case OP_MINUS: return "OP_MINUS";
        case OP_STAR: return "OP_STAR";
        case OP_SLASH: return "OP_SLASH";
        case OP_PERCENT: return "OP_PERCENT";
        case OP_ASSIGN: return "OP_ASSIGN";
        case OP_EQ: return "OP_EQ";
        case OP_NE: return "OP_NE";
        case SEP_LPAREN: return "SEP_LPAREN";
        case SEP_RPAREN: return "SEP_RPAREN";
        case SEP_LBRACKET: return "SEP_LBRACKET";
        case SEP_RBRACKET: return "SEP_RBRACKET";
        case SEP_LBRACE: return "SEP_LBRACE";
        case SEP_RBRACE: return "SEP_RBRACE";
        case SEP_SEMICOLON: return "SEP_SEMICOLON";
        case SEP_DOT: return "SEP_DOT";
        case SEP_COMMA: return "SEP_COMMA";
        case STRING_LITERAL: return "STRING_LITERAL";
        case END_OF_FILE: return "END_OF_FILE";
        default: return "UNDEF";
    }
}

int main(int argc, char* argv[]) 
{
    // Verifica se o nome do arquivo foi passado [cite: 12]
    if (argc != 2)
    {
        cout << "Uso: ./xpp_compiler nome_arquivo.xpp\n";
        return 1;
    }

    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    
    do
    {
        t = scanner->nextToken();
        
        cout << "<" << tokenNameToString(t->name) << ", \"" << t->lexeme << "\">" << endl;

    } while (t->name != END_OF_FILE);

    // Etapa 2 (Análise Sintática) informaria "Compilação encerrada com sucesso" [cite: 65]
    // A Etapa 1 apenas lista os tokens.

    delete scanner;
    
    return 0;
}