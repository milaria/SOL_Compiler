//http://en.wikipedia.org/wiki/List_of_prime_numbers
#include "hash_table.h"
#include "parser.h"
#include "semantic.h"

int hash (char* id){
    int i, h = 0;
    for(i = 0; id[i] != '\0'; i++){
        h = ((h << SHIFT) + id[i]) % TOT;
    }
    return h;
}

int insert(Phash_node p, Phash_node * table){
    if (getNode(p->name, table) != NULL) {
        return 0;
    }
    int pos = hash(p->name);
    p->next = table[pos];
    table[pos] = p;
    return 1;
}

int insert_func(Phash_node p, Phash_node f_parent){
    int not_used;
    if (find_visible_node(p->name, f_parent, &not_used) != NULL) {
        return 0;
    }
    return insert(p, f_parent->aux->locenv);
}

Phash_node getNode(char * id, Phash_node * table){
    int pos = hash(id);
    Phash_node node = table[pos];
    while(node){
        if (strcmp(node->name,id) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

Phash_node find_visible_node(char * id, Phash_node func_node, int * offset){
    Phash_node f = func_node;
    printf("looking for: %s\n", id);
    *offset=0;
    while (f != NULL) {
        Phash_node node = getNode(id, f->aux->locenv);
        if (node != NULL) {
            //print_generic_node(node);
            //printSchema(node->schema, " ");
            return node;
        }
        f = f->father;
        (*offset)++;
    }
    if (f == NULL) {
        if (strcmp(func_node->name,id) == 0) {
            return func_node;
        }
    }
    return NULL;
}

Phash_node * new_hash_table(){
    //Phash_node * table = (Phash_node *) malloc(TOT * sizeof(Phash_node));
    //memset(table, '\0', TOT * sizeof(Phash_node)); // NULL == '\0'
    Phash_node * table = (Phash_node *) calloc(TOT, sizeof(Phash_node));
    return table;
}

void print_func_node(Phash_node node){
    printf("[%d] %s | ", node->oid, node->name);
    if (node->class_node == CLFUNC) {
        printf("FUNC ");
        (node->schema != NULL)? printf("ok schema |"): printf("no schema |");
        ((node->aux)->locenv != NULL)? printf("ok env |"): printf("no env |");
        printf ("%d :", (node->aux)->formals_num);
        
        Formal * f = (node->aux)->formal;
        while (f!= NULL) {
            printf ("[%d]", f->formal->oid);
            printf ("(%s) ", f->formal->name);
            f = f->next;
        }
        if(node->father)
            printf(" | F_oid: %d", node->father->oid);
        else
            printf(" | F_oid: NULL");
    }
    printf ("\n");
}

void print_generic_node(Phash_node node){
    printf("[%d] %s | ", node->oid, node->name);
    
    switch (node->class_node) {
        case CLTYPE:
            printf("TYPE ");
            break;
        case CLVAR:
            printf("VAR ");
            break;
        case CLCONST:
            printf("CONST ");
            break;
        case CLPAR:
            printf("PAR ");
            break;
        case CLCOUNT:
            printf("COUNT ");
            break;
        default:
            //function... TODO merge with print_func_node OR not?
            break;
    }
    (node->schema != NULL)? printf("ok schema |"): printf("no schema |");
    
    printf ("\n");
}


void printSchema(Pschema root, char* father_indent){
    if(root==NULL) return;
    Pschema p;
    
    //printf("[][][][][][]\n");
    //printf("%s\n",schema2format(root));
    
    //--Indent Stuff
    char* my_indent = calloc(1000,sizeof(char));
    my_indent[0]=0;
    strcpy(my_indent, father_indent);
    
    if(root->p2)
        //strcat(my_indent,"    ├");
        strcat(my_indent,"├");
    else
        //strcat(my_indent,"    └");
        strcat(my_indent,"└");
    printf("%s─", my_indent);
    
    //--PRINTING SINGLE NODE
    switch(root->type){
        case VECTOR:
            printf("%s [ %s ] [ %d ]", "VECTOR", root->id, root->size);
            break;
        case STRUCT:
            printf("%s [ %s ] [ - ]", "STRUCT", root->id);
            break;
        case CHAR:
            printf("%s [ %s ] [ - ]", "CHAR", root->id);
            break;
        case INT:
            //printf("\n## %d\n",INT);
            printf("%s [ %s ] [ - ]", "INT", root->id);
            break;
        case REAL:
            printf("%s [ %s ] [ - ]", "REAL", root->id);
            break;
        case STRING:
            printf("%s [ %s ] [ - ]", "STRING", root->id);
            break;
        case BOOL:
            printf("%s [ %s ] [ - ]", "BOOL", root->id);
            break;
            // case ATTR:
            //     printf(" %s [ - ] [ - ]", "ATTR");
            //     break;
        default:
            printf("ERROR\n");
            break;
    }
    
    //--OTHER INDENT AND RECURSION AND BROTHER CICLE
    printf("\n");
    for(p=root->p1; p != NULL; p = p->p2){
        my_indent[strlen(my_indent)-4]=0;
        strcat(my_indent," |  ");
        if(root->p2 == NULL){
            my_indent[strlen(my_indent)-4]=0;
            strcat(my_indent,"    ");
        }
        printSchema(p, my_indent);
    }
    free(my_indent);
}

void print_sch(Pschema root){
    printSchema(root, " ");
    printf("\n");
}

void print_hash_content(Phash_node * table){
    int i;
    Phash_node temp;
    printf("-------------local environment\n");
    for (i=0; i<TOT; i++){
        temp = table[i];
        while (temp != NULL) {
            printf("%3d. ", i);
            print_generic_node(temp);
            if(temp->schema != NULL){
                printSchema(temp->schema, "   ");
            }
            temp = temp->next;
        }
    }
    printf("-------------end\n");
}



/*
 typedef struct shash_node{
 char * name;
 int oid;
 Class class_node; //TYPE, VAR, CONST, FUNC, PAR
 Pschema schema;
 struct shash_node ** locenv;
 int formals_num;
 Formal * formal;
 struct shash_node * next;
 } Hash_node, * Phash_node;
 */