#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    pos = 0;
    line = 1;
    initReservedWords();

    ifstream inputFile(input, ios::in);
    string fileLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, fileLine))
        {
            this->input.append(fileLine + '\n');
        }
        inputFile.close();
    }
    else 
        lexicalError("Unable to open file");
}

void Scanner::initReservedWords()
{
    reservedWords["class"] = CLASS;
    reservedWords["extends"] = EXTENDS;
    reservedWords["int"] = INT;
    reservedWords["string"] = STRING;
    reservedWords["break"] = BREAK;
    reservedWords["print"] = PRINT;
    reservedWords["read"] = READ;
    reservedWords["return"] = RETURN;
    reservedWords["super"] = SUPER;
    reservedWords["if"] = IF;
    reservedWords["else"] = ELSE;
    reservedWords["for"] = FOR;
    reservedWords["new"] = NEW;
    reservedWords["constructor"] = CONSTRUCTOR;
}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    string lexeme;

    while (pos < input.length())
    {
        char currentChar = input[pos];

        if (isspace(currentChar))
        {
            if (currentChar == '\n')
                line++;
            pos++;
            continue;
        }

        // Comentários
        if (currentChar == '/')
        {
            if (pos + 1 < input.length() && input[pos + 1] == '/')
            {
                pos += 2;
                while (pos < input.length() && input[pos] != '\n')
                {
                    pos++;
                }
                continue; // Ignora comentário de linha
            }
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

        // Identificadores e Palavras Reservadas
        if (isalpha(currentChar) || currentChar == '_')
        {
            lexeme += currentChar;
            pos++;
            while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
            {
                lexeme += input[pos];
                pos++;
            }
            if (reservedWords.count(lexeme))
            {
                return new Token(reservedWords[lexeme], lexeme);
            }
            return new Token(ID, lexeme);
        }

        // Números inteiros
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

        // Literais de string
        if (currentChar == '"')
        {
            pos++; // Consome '"'
            while (pos < input.length() && input[pos] != '"')
            {
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

        // Operadores e Separadores
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
                lexicalError("Token inválido: !.");
                break;
            case '+': pos++; return new Token(OP_PLUS, "+");
            case '-': pos++; return new Token(OP_MINUS, "-");
            case '*': pos++; return new Token(OP_STAR, "*");
            case '%': pos++; return new Token(OP_PERCENT, "%");
            case '/': pos++; return new Token(OP_SLASH, "/");
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
                lexicalError("Caractere inválido: " + string(1, currentChar));
                break;
        }
    }

    return new Token(END_OF_FILE, "");
 
}


Token* Scanner::peekToken(int k)
{
    // Cria uma cópia temporária do scanner para realizar o lookahead
    Scanner tempScanner = *this;
    Token* token = nullptr;
    for (int i = 0; i < k; ++i) {
        token = tempScanner.nextToken();
    }
    return token;
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
