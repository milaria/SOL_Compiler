%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "s_exec.h"
#include "s_machine.h"
#include "../s_compiler/schema.h"

#define YYSTYPE Pschema

extern Value lexval;
extern Str_c_node ** format_stringtable;
Pschema root = NULL;
char * temp_s;
int temp_i;

%}
%token FORMAT_LEX_ID FORMAT_LEX_INT FORMAT_LEX_ERROR
%%

format_string : format {root = $$; $$ = $1;}

format : atomic_format
        | struct_format
        | vector_format

atomic_format   : 'c' {$$ = new_schema_node(SCCHAR);}
                | 'i' {$$ = new_schema_node(SCINT);}
                | 'r' {$$ = new_schema_node(SCREAL);}
                | 's' {$$ = new_schema_node(SCSTRING);}
                | 'b' {$$ = new_schema_node(SCBOOL);}

struct_format : '(' attr attr_list ')' {$$ = new_schema_node(SCSTRUCT);
                                        $$->p1 = $2;
                                        $2->p2 = $3;}

attr_list : ',' attr attr_list {$$ = $2;
                                $2->p2 = $3;}
        | /*eps*/ {$$ = NULL;}

attr : FORMAT_LEX_ID {temp_s = lexval.sval;} format {$$ = $3; $3->id = temp_s;}

vector_format : '[' FORMAT_LEX_INT {$$ = new_schema_node(SCVECTOR); $$->size = lexval.ival;} ',' format ']' {$$ = $3; $$->p1 = $5;}

%%

int yyerror(){
    fprintf(stderr, "Error while parsing the format.\n");
    return -1;
}

Pschema parse_format(char * format){
    int result;
    format_scan_string(format,strlen(format));
    if((result = formatparse()) == 0)
        print_sch(root);
    
    return root;
}