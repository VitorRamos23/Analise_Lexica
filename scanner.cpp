#include "scanner.h"    

//Construtor
Scanner::Scanner(string input, SymbolTable* table)
{
    pos = 0;
    line = 1;

    st = table; // Armazena a tabela de símbolos recebida

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

            // ***ETAPA 3 ***
            // Pesquisa na tabela de símbolos (que contém as palavras reservadas)
            STEntry* obj = st->get(lexeme);
            
            if (!obj)
            {
                // Não encontrou, é um ID comum
                return new Token(ID, lexeme);
            }
            else 
            {
                // Encontrou, é uma palavra reservada.
                // Retorna um token com o nome correto (ex: CLASS, IF, FOR)
                return new Token(obj->token->name, lexeme);
            }
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

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}


Token* Scanner::peekToken(int n)
{
    // 1. Salvar o estado atual do scanner
    std::string::size_type oldPos = pos;
    int oldLine = line;

    // 2. Scanear 'n' tokens à frente
    Token* t = nullptr;
    std::vector<Token*> temp_tokens; // Armazena tokens intermediários

    for (int i = 0; i < n; i++)
    {
        t = this->nextToken(); // nextToken() aloca um novo token
        temp_tokens.push_back(t);
        if (t->name == END_OF_FILE)
            break;
    }

    // 3. 't' é o n-ésimo token. Criamos uma *cópia* dele
    //    para o parser, pois os tokens temporários serão 
    //    deletados.
    Token* result;
    if (t) {
        // Assume que Token(name, lexeme) é o construtor
        result = new Token(t->name, t->lexeme); 
    } else {
        // Caso de erro ou n=0
        result = new Token(END_OF_FILE, "");
    }

    // 4. Limpar todos os tokens temporários que foram alocados
    for (Token* temp_t : temp_tokens)
    {
        delete temp_t;
    }

    // 5. Restaurar o estado original do scanner
    pos = oldPos;
    line = oldLine;

    // 6. Retornar a cópia do token
    return result;
}
