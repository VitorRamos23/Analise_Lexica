#include "symboltable.h"
#include <fstream>
#include <cstdlib> // Para exit()
#include <vector> // <--- ADICIONADO (Necessário para peekToken)

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        std::string::size_type pos;//Posição atual
        int line;
        SymbolTable* st; // Tabela de símbolos
    
    public:
    //Construtor
        Scanner(string, SymbolTable*); // Recebe a TS do Parser

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
        Token* peekToken(int n); // <--- ADICIONADO

        //Método para manipular erros
        void lexicalError(string);
};
