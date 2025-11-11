
#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
        SymbolTable* globalST;  // Tabela de símbolos global
    	SymbolTable* currentST; // Tabela de símbolos do escopo atual

		void advance();
		void match(int);
		void error(string);
		void initSimbolTable(); // Novo método para carregar palavras reservadas

        // Métodos do parser da Etapa 2
		void program();
		void classList();
		void classDecl();
		void classBody();
        void varDeclListOpt();
        void varDeclList();
        void varDecl();
        void varDeclOpt();
        void type();
        void constructDeclListOpt();
        void constructDeclList();
        void constructDecl();
        void methodDeclListOpt();
        void methodDeclList();
        void methodDecl();
        void methodBody();
        void paramListOpt();
        void paramList();
        void param();
        void statementsOpt();
        void statements();
        void statement();
        void atribOrCallStat();
        void printStat();
        void readStat();
        void returnStat();
        void superStat();
        void ifStat();
        void forStat();
        void atribStatOpt();
        void expressionOpt();
        void lValue();
        void lValueComp();
        void expression();
        void allocExpression();
        void numExpression();
        void term();
        void unaryExpression();
        void factor();
        void argListOpt();
        void argList();

	public:
		Parser(string);
		void run();
};
