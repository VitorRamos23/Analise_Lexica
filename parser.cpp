// ... (Includes e métodos do início do parser.cpp) ...

void
Parser::readStat()
{
    match(READ);
    // // A gramática original `gramatica_x++.pdf` [cite: 63] diz `read LValue`
    // // A gramática `etapa2_sintatica_2025_2.pdf` [cite: 201] também.
    // O parser da Etapa 2 implementava `read(lValue)`.
    // <--- CORRIGIDO: Removendo parênteses para seguir a gramática --->
    // match(SEP_LPAREN); // <--- REMOVIDO
    lValue();
    // match(SEP_RPAREN); // <--- REMOVIDO
}

void
Parser::returnStat()
{
// ... (código do returnStat sem alterações) ...
}

// ... (código de superStat, ifStat, forStat, etc.) ...

void
Parser::lValueComp()
{
    if (lToken->name == SEP_DOT)
    {
        match(SEP_DOT);
        match(ID);

        // <--- CORRIGIDO: Bloco de chamada de método (SEP_LPAREN) removido --->
        // A lógica de `atribOrCallStat` já trata a chamada de método
        // (o 'else if (lToken->name == SEP_LPAREN)' lá).
        // Manter isso aqui consumia os parênteses e quebrava `atribOrCallStat`.
        /* if (lToken->name == SEP_LPAREN) // Method call
        {
            match(SEP_LPAREN);
            argListOpt();
            match(SEP_RPAREN);
        }
        */
        
        // <--- CORRIGIDO: O 'else if' original estava logicamente errado --->
        // `obj.array[0]` falharia, pois `else if` não seria checado
        // após `match(ID)`. Agora é um `if` independente.
        if (lToken->name == SEP_LBRACKET) // Array access
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
}

// ... (Restante do parser.cpp) ...
