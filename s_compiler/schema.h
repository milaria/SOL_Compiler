#ifndef SCHEMA
#define SCHEMA

#include <string.h>
#include "def.h"

typedef struct sschema{
    int type; //CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR (ATTR no more)
    char * id; //field name
    int size; //array size (type = VECTOR)
    struct sschema *p1, *p2;
} Schema, * Pschema;

void printSchema(Pschema root, char* father_indent);
void print_sch(Pschema root);

Pschema new_schema_node(int _type);
int are_compatible(Pschema a, Pschema b);

int compute_size(Pschema schema);
char * schema2format(Pschema schema);
char * make_format(Pschema schema);

#endif