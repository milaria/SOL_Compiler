#ifndef HASH_TABLE
#define HASH_TABLE

#define TOT 3517
#define SHIFT 4
#include <string.h>
#include "def_y.h"

typedef enum{
    CLTYPE,
    CLVAR,
    CLCONST,
    CLFUNC,
    CLPAR
} Class;


typedef struct sformalsnode{
    struct shash_node * formal;
    struct sformalsnode * next;
} Formal;

typedef struct sschema{
    int type; //CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR, ATTR
	char * id; //field name
	int size; //array size (type = VECTOR)
	struct sschema *p1, *p2;
} Schema, * Pschema;

typedef struct shash_node{
    char * name;
    int oid;
    Class class_node; //TYPE, VAR, CONST, FUNC, PAR
    Schema * schema;
    struct shash_node ** locenv;
    int formals_num;
    Formal * formal;
    struct shash_node * next;
} Hash_node, * Phash_node;

int hash (char* id);
void insert(Phash_node p,Phash_node * table);
Phash_node getNode(char * id, Phash_node * table);
Phash_node * new_hash_table();
#endif