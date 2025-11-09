#include "parser.h"
#include <cstdlib> // Para exit()
#include <iostream>

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
	if (lToken->name == CLASS)
    	classList();

    if (lToken->name != END_OF_FILE)
    {
        error("Tokens inesperados após a definição do programa.");
    }
}

void
Parser::classList()
{
    classDecl();
    if (lToken->name == CLASS)
    {
        classList();
    }
}

void
Parser::classDecl()
{
    match(CLASS);
    match(ID);
    if (lToken->name == EXTENDS)
    {
        match(EXTENDS);
        match(ID);
    }
    classBody();
}

void
Parser::classBody()
{
    match(SEP_LBRACE);
    varDeclListOpt();
    constructDeclListOpt();
    methodDeclListOpt();
    match(SEP_RBRACE);
}

void
Parser::varDeclListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        varDeclList();
    }
}

void
Parser::varDeclList()
{
    varDecl();
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        varDeclList();
    }
}

void
Parser::varDecl()
{
    type();
    if (lToken->name == SEP_LBRACKET)
    {
        match(SEP_LBRACKET);
        match(SEP_RBRACKET);
    }
    match(ID);
    varDeclOpt();
    match(SEP_SEMICOLON);
}

void
Parser::varDeclOpt()
{
    if (lToken->name == SEP_COMMA)
    {
        match(SEP_COMMA);
        match(ID);
        varDeclOpt();
    }
}

void
Parser::type()
{
    if (lToken->name == INT)
    {
        match(INT);
    }
    else if (lToken->name == STRING)
    {
        match(STRING);
    }
    else if (lToken->name == ID)
    {
        match(ID);
    }
    else
    {
        error("Esperado tipo (int, string ou ID).");
    }
}

void
Parser::constructDeclListOpt()
{
    if (lToken->name == CONSTRUCTOR)
    {
        constructDeclList();
    }
}

void
Parser::constructDeclList()
{
    constructDecl();
    if (lToken->name == CONSTRUCTOR)
    {
        constructDeclList();
    }
}

void
Parser::constructDecl()
{
    match(CONSTRUCTOR);
    methodBody();
}

void
Parser::methodDeclListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        methodDeclList();
    }
}

void
Parser::methodDeclList()
{
    methodDecl();
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        methodDeclList();
    }
}

void
Parser::methodDecl()
{
    type();
    if (lToken->name == SEP_LBRACKET)
    {
        match(SEP_LBRACKET);
        match(SEP_RBRACKET);
    }
    match(ID);
    methodBody();
}

void
Parser::methodBody()
{
    match(SEP_LPAREN);
    paramListOpt();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statementsOpt();
    match(SEP_RBRACE);
}

void
Parser::paramListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        paramList();
    }
}

void
Parser::paramList()
{
    param();
    if (lToken->name == SEP_COMMA)
    {
        match(SEP_COMMA);
        paramList();
    }
}

void
Parser::param()
{
    type();
    if (lToken->name == SEP_LBRACKET)
    {
        match(SEP_LBRACKET);
        match(SEP_RBRACKET);
    }
    match(ID);
}

void
Parser::statementsOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
        lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
        lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
        lToken->name == BREAK || lToken->name == SEP_SEMICOLON)
    {
        statements();
    }
}

void
Parser::statements()
{
    statement();
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
        lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
        lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
        lToken->name == BREAK || lToken->name == SEP_SEMICOLON)
    {
        statements();
    }
}

void
Parser::statement()
{
    if (lToken->name == INT || lToken->name == STRING)
    {
        varDecl();
    }
    else if (lToken->name == ID)
    {
        Token* next = scanner->peekToken(1); // Lookahead 1
        Token* next2 = scanner->peekToken(2); // Lookahead 2

        if (lToken->name == ID) { // Se o token atual é um ID
            if (next->name == ID) { // ID ID -> VarDecl
                varDecl();
            } else if (next->name == SEP_LBRACKET && next2->name == SEP_RBRACKET) { // ID [ ] -> VarDecl (ex: int[] arr;)
                varDecl();
            } else { // ID = ou ID ( ou ID [ Expression ] = -> AtribStat
                atribOrCallStat();
                match(SEP_SEMICOLON);
            }
        } else { // Se o token atual não é ID, mas é um Type (int, string, etc.)
            varDecl();
        }
        delete next;
        delete next2;
    }
    else if (lToken->name == NEW)
    {
        allocExpression();
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == PRINT)
    {
        printStat();
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == READ)
    {
        readStat();
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == RETURN)
    {
        returnStat();
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == SUPER)
    {
        superStat();
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == IF)
    {
        ifStat();
    }
    else if (lToken->name == FOR)
    {
        forStat();
    }
    else if (lToken->name == BREAK)
    {
        match(BREAK);
        match(SEP_SEMICOLON);
    }
    else if (lToken->name == SEP_SEMICOLON)
    {
        match(SEP_SEMICOLON);
    }
    else
    {
        error("Esperado início de declaração ou comando.");
    }
}

void Parser::atribOrCallStat()
{
    lValue();
    if (lToken->name == OP_ASSIGN)
    {
        match(OP_ASSIGN);
        expression();
    }
    else if (lToken->name == SEP_LPAREN)
    {
        match(SEP_LPAREN);
        argListOpt();
        match(SEP_RPAREN);
    }
    else
    {
        error("Esperado '=' ou '(' após LValue em um comando.");
    }
}

void
Parser::printStat()
{
    match(PRINT);
    expression();
}

void
Parser::readStat()
{
    match(READ);
    match(SEP_LPAREN);
    lValue();
    match(SEP_RPAREN);
}

void
Parser::returnStat()
{
    match(RETURN);
    if (lToken->name != SEP_SEMICOLON) // Se não for ponto e vírgula, espera uma expressão
    {
        expression();
    }
}

void
Parser::superStat()
{
    match(SUPER);
    match(SEP_LPAREN);
    argListOpt();
    match(SEP_RPAREN);
}

void
Parser::ifStat()
{
    match(IF);
    match(SEP_LPAREN);
    expression();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statements();
    match(SEP_RBRACE);
    if (lToken->name == ELSE)
    {
        match(ELSE);
        match(SEP_LBRACE);
        statements();
        match(SEP_RBRACE);
    }
}

void
Parser::forStat()
{
    match(FOR);
    match(SEP_LPAREN);
    atribStatOpt();
    match(SEP_SEMICOLON);
    expressionOpt();
    match(SEP_SEMICOLON);
    atribStatOpt();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statements();
    match(SEP_RBRACE);
}

void
Parser::atribStatOpt()
{
    if (lToken->name == ID)
    {
        atribOrCallStat();
    }
}

void
Parser::expressionOpt()
{
    if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN ||
        lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        expression();
    }
}

void
Parser::lValue()
{
    match(ID);
    lValueComp();
}

void
Parser::lValueComp()
{
    if (lToken->name == SEP_DOT)
    {
        match(SEP_DOT);
        match(ID);
        if (lToken->name == SEP_LPAREN) // Method call
        {
            match(SEP_LPAREN);
            argListOpt();
            match(SEP_RPAREN);
        }
        else if (lToken->name == SEP_LBRACKET) // Array access
        {
            match(SEP_LBRACKET);
            expression();
            match(SEP_RBRACKET);
        }
        lValueComp();
    }
    else if (lToken->name == SEP_LBRACKET) // Array access
    {
        match(SEP_LBRACKET);
        expression();
        match(SEP_RBRACKET);
        lValueComp();
    }
    // Se não houver mais . ou [], a recursão para aqui.
}

void
Parser::expression()
{
    numExpression();
    if (lToken->name == OP_LT || lToken->name == OP_GT ||
        lToken->name == OP_LE || lToken->name == OP_GE ||
        lToken->name == OP_EQ || lToken->name == OP_NE)
    {
        match(lToken->name);
        numExpression();
    }
}

void
Parser::allocExpression()
{
    match(NEW);
    if (lToken->name == ID)
    {
        match(ID);
        match(SEP_LPAREN);
        argListOpt();
        match(SEP_RPAREN);
    }
    else if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        type();
        match(SEP_LBRACKET);
        expression();
        match(SEP_RBRACKET);
    }
    else
    {
        error("Esperado ID ou tipo após NEW.");
    }
}

void
Parser::numExpression()
{
    term();
    while (lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        match(lToken->name);
        term();
    }
}

void
Parser::term()
{
    unaryExpression();
    while (lToken->name == OP_STAR || lToken->name == OP_SLASH || lToken->name == OP_PERCENT)
    {
        match(lToken->name);
        unaryExpression();
    }
}

void
Parser::unaryExpression()
{
    if (lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        match(lToken->name);
    }
    factor();
}

void
Parser::factor()
{
    if (lToken->name == INTEGER_LITERAL)
    {
        match(INTEGER_LITERAL);
    }
    else if (lToken->name == STRING_LITERAL)
    {
        match(STRING_LITERAL);
    }
    else if (lToken->name == ID)
    {
        lValue();
    }
    else if (lToken->name == SEP_LPAREN)
    {
        match(SEP_LPAREN);
        expression();
        match(SEP_RPAREN);
    }
    else if (lToken->name == NEW)
    {
        allocExpression();
    }
    else
    {
        error("Esperado literal inteiro, literal string, ID, expressão entre parênteses ou 'new'.");
    }
}

void
Parser::argListOpt()
{
    if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN ||
        lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        argList();
    }
}

void
Parser::argList()
{
    expression();
    if (lToken->name == SEP_COMMA)
    {
        match(SEP_COMMA);
        argList();
    }
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}