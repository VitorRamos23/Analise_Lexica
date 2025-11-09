#include "scanner.h"
#include <cstdlib> // Para exit()    
#include <iostream> // Para cout

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    // cout << this->input; // Descomente para depurar a entrada

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    while (pos < input.length())
    {
        char currentChar = input[pos];

        // Ignora espaços em branco 
        if (isspace(currentChar))
        {
            if (currentChar == '\n')
                line++;
            pos++;
            continue;
        }

        // Reconhece Identificadores (ID) [cite: 196]
        if (isalpha(currentChar) || currentChar == '_')
        {
            lexeme += currentChar;
            pos++;
            while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
            {
                lexeme += input[pos];
                pos++;
            }
            // Nota: Palavras reservadas são tratadas como ID nesta etapa [cite: 204]
            return new Token(ID, lexeme);
        }

        // Reconhece INTEGER_LITERAL [cite: 198]
        if (isdigit(currentChar))
        {
            lexeme += currentChar;
            pos++;
            while (pos < input.length() && isdigit(input[pos]))
            {
                lexeme += input[pos];
                pos++;
            }
            return new Token(INTEGER_LITERAL, lexeme);
        }

        // Reconhece STRING_LITERAL [cite: 201]
        if (currentChar == '"')
        {
            pos++; // Consome '"'
            while (pos < input.length() && input[pos] != '"')
            {
                if (input[pos] == '\n') // Strings não podem pular linha
                    lexicalError("String literal com quebra de linha.");
                lexeme += input[pos];
                pos++;
            }
            if (pos < input.length() && input[pos] == '"')
            {
                pos++; // Consome '"'
                return new Token(STRING_LITERAL, lexeme);
            }
            else
            {
                lexicalError("String literal não fechada.");
            }
        }

        // Reconhece e ignora Comentários 
        if (currentChar == '/')
        {
            // Comentário de linha
            if (pos + 1 < input.length() && input[pos + 1] == '/')
            {
                pos += 2;
                while (pos < input.length() && input[pos] != '\n')
                {
                    pos++;
                }
                continue; // Ignora comentário de linha
            }
            // Comentário de bloco
            else if (pos + 1 < input.length() && input[pos + 1] == '*')
            {
                pos += 2;
                while (pos + 1 < input.length() && !(input[pos] == '*' && input[pos + 1] == '/'))
                {
                    if (input[pos] == '\n')
                        line++;
                    pos++;
                }
                if (pos + 1 < input.length())
                {
                    pos += 2; // Consome '*/'
                    continue; // Ignora comentário de bloco
                }
                else
                {
                    lexicalError("Comentário de bloco não fechado.");
                }
            }
        }

        // Reconhece Operadores e Separadores 
        switch (currentChar)
        {
            case '<':
                if (pos + 1 < input.length() && input[pos + 1] == '=')
                {
                    pos += 2;
                    return new Token(OP_LE, "<=");
                }
                pos++;
                return new Token(OP_LT, "<");
            case '>':
                if (pos + 1 < input.length() && input[pos + 1] == '=')
                {
                    pos += 2;
                    return new Token(OP_GE, ">=");
                }
                pos++;
                return new Token(OP_GT, ">");
            case '=':
                if (pos + 1 < input.length() && input[pos + 1] == '=')
                {
                    pos += 2;
                    return new Token(OP_EQ, "==");
                }
                pos++;
                return new Token(OP_ASSIGN, "=");
            case '!':
                if (pos + 1 < input.length() && input[pos + 1] == '=')
                {
                    pos += 2;
                    return new Token(OP_NE, "!=");
                }
                lexicalError("Token inválido (!).");
                break;
            case '+': pos++; return new Token(OP_PLUS, "+");
            case '-': pos++; return new Token(OP_MINUS, "-");
            case '*': pos++; return new Token(OP_STAR, "*");
            case '/': pos++; return new Token(OP_SLASH, "/");
            case '%': pos++; return new Token(OP_PERCENT, "%");
            case '(': pos++; return new Token(SEP_LPAREN, "(");
            case ')': pos++; return new Token(SEP_RPAREN, ")");
            case '[': pos++; return new Token(SEP_LBRACKET, "[");
            case ']': pos++; return new Token(SEP_RBRACKET, "]");
            case '{': pos++; return new Token(SEP_LBRACE, "{");
            case '}': pos++; return new Token(SEP_RBRACE, "}");
            case ';': pos++; return new Token(SEP_SEMICOLON, ";");
            case '.': pos++; return new Token(SEP_DOT, ".");
            case ',': pos++; return new Token(SEP_COMMA, ",");
            default:
                lexicalError("Caractere inválido.");
                break;
        }
    }

    tok = new Token(END_OF_FILE, "");

    return tok;
 
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}