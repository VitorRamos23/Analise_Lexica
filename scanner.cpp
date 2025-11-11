#include "scanner.h"    
// <... todo o código de scanner.cpp ...>
// (Construtor, getLine, nextToken, lexicalError)
// ...
// ... (fim do nextToken e lexicalError) ...

// <--- INÍCIO DA IMPLEMENTAÇÃO ADICIONADA --->
//
// Implementação de peekToken(n)
// Este método "espia" o n-ésimo token à frente sem 
// consumir a entrada.
//
// NOTA: Esta implementação assume que seu 'token.h'
// define 'Token' com um construtor 'Token(int name, string lexeme)'
// e membros públicos 'name' e 'lexeme'.
//
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
// <--- FIM DA IMPLEMENTAÇÃO ADICIONADA --->
