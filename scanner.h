#include <fstream>
#include <cstdlib> // Para exit()
#include "token.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        std::string::size_type pos;//Posição atual
        int line;
    
    public:
    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};