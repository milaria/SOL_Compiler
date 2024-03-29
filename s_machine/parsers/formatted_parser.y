/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief parser for formatted data, to parsering saved file or console input.
 */


%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "../s_machine.h"
    #include "../sol_io.h"
    
    #define YYSTYPE Pformatted

    extern FILE *formattedin;
    extern Value lexval;
    extern Str_c_node ** format_stringtable;
    extern int formattedlex();
    extern void formattedrestart(FILE *);
    extern int yyerror();
    Pformatted formatted_root = NULL;
    
    Pformatted newnode(Typeformatted tnode){
        Pformatted p = newmem(sizeof(Formatted));
        p->type = tnode;
        p->child = p->brother = NULL;
        p->id = NULL;
        return(p);
    }
    
    Pformatted vectornode(){
        Pformatted p = newnode(F_VECTOR);
        return(p);
    }
    
    Pformatted structnode(){
        Pformatted p = newnode(F_STRUCT);
        return(p);
    }
    Pformatted iconstnode(){
        Pformatted p = newnode(F_INTCONST);
        p->value.ival = lexval.ival;
        return(p);
    }
    
    Pformatted cconstnode(){
        Pformatted p = newnode(F_CHARCONST);
        p->value.ival = lexval.cval;
        return(p);
    }
    
    Pformatted rconstnode(){
        Pformatted p = newnode(F_REALCONST);
        p->value.rval = lexval.rval;
        return(p);
    }
    
    Pformatted sconstnode(){
        Pformatted p = newnode(F_STRCONST);
        //p->value.sval = insert_strWclean(lexval.sval);
        p->value.sval = lexval.sval;
        return(p);
    }
    
    Pformatted bconstnode(){
        Pformatted p = newnode(F_BOOLCONST);
        p->value.cval = lexval.cval;
        return(p);
    }
    

    
    
    %}
%token FORMATTED_LEX_ID CHARCONST INTCONST REALCONST STRCONST BOOLCONST FORMATTED_LEX_ERROR
%%

formatted_string : formatted {formatted_root = $$; $$ = $1;}

formatted   : atomic_formatted
            | struct_formatted
            | vector_formatted

atomic_formatted    : CHARCONST {$$ = cconstnode();}
                    | INTCONST  {$$ = iconstnode();}
                    | REALCONST {$$ = rconstnode();}
                    | STRCONST  {$$ = sconstnode();}
                    | BOOLCONST {$$ = bconstnode();}

struct_formatted :  '(' attr_list ')' {$$ = structnode(); $$->child = $2;}

attr_list   : attr ',' attr_list {$$ = $1; $$->brother = $3;}
            | attr

attr : FORMATTED_LEX_ID {$$ = newnode(F_TEMP); $$->id = lexval.sval;} formatted {$$ = $3; $3->id = $2->id; freemem($2, sizeof(Formatted));}

vector_formatted : '[' formatted_list ']' {$$ = vectornode(); $$->child = $2;}

formatted_list  : formatted ',' formatted_list {$$ = $1; $1->brother = $3;}
                | formatted


%%

int yyerror(){
    fprintf(stderr, "Error while parsing the formatted values.\n");
    formatted_root=NULL;
    return -1;
}

void parse_formatted(FILE* stream){
    formattedin=stream;
    formattedrestart(formattedin);
    yyparse();
}

