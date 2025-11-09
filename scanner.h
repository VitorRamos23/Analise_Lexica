#include <fstream>
#include <cstdlib> // Para exit()
#include <map>
#include "token.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        std::string::size_type pos;//Posição atual
        int line;
        map<string, int> reservedWords;
    
    public:
    //Construtor
        Scanner(string);
        void initReservedWords();

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
        Token* peekToken(int k = 1);        
    
        //Método para manipular erros
        void lexicalError(string);
};
