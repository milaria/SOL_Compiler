/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief This contains all function to manage the symbol table.
 */

#include "symbol_table.h"
#include "parser/parser.h"
#include "semantic.h"

#define TABLE 0

int oid = 1;
char error_msg_symb[100];

void handle_function_part(Pnode current, Phash_node func, Class part_class, int * num_obj){
    if (current->child != NULL) { //?_SECT_OPT
        Pnode child;
        child = current->child; //DECL
        while (child != NULL) {
            Pnode id_list = child->child;
            Pschema domain_sch = create_schema(id_list->brother, func, NULL);
            
            Pnode id = id_list->child;
            while (id != NULL){
                Phash_node id_node = new_id_node(id->value.sval, part_class, *num_obj + 1);
                id_node->schema = domain_sch;
                (*num_obj)++;
                
                if(!insert(id_node, (func->aux)->locenv)){
                    sprintf(error_msg_symb, "ID \"%s\" already defined in same environment\n", id->value.sval);
                    sem_error(id, error_msg_symb);
                }
                id = id->brother;
            }
            
            child = child->brother;
            if (part_class == CLCONST)
                child = child->brother;
        }
    }
}

Phash_node create_symbol_table(Pnode root, Phash_node father){
    Pnode current, child;
    switch (root->type) {
        case T_NONTERMINAL:
            switch (root->value.ival) {
                case NPROGRAM:
                    return create_symbol_table(root->child, father);
                    break;
                case NFUNC_DECL:
                    current = root->child; //ID
                    
                    Phash_node func = new_function_node(current->value.sval);
                    func->father = father;
                    Phash_node * loc = new_hash_table();
                    (func->aux)->locenv = loc;
                    
                    (func->aux)->formals_num = 0;
                    current = current->brother; //DECL_LIST_OPT
                    
                    if (current->child != NULL) {//handle parameters
                        child = current->child; //DECL
                        Formal * last_formal = (func->aux)->formal;
                        while (child != NULL) { //loop on DECL
                            Pnode id_list = child->child;
                            Pschema domain_sch = create_schema(id_list->brother, func, NULL);
                            
                            Pnode id = id_list->child;
                            while (id != NULL){ //loop on IDs
                                Phash_node id_node = new_id_node(id->value.sval, CLPAR, ((func->aux)->formals_num)+1);
                                id_node->schema = domain_sch;
                                if (!insert(id_node, (func->aux)->locenv)) {
                                    sprintf(error_msg_symb, "Name of formal parameters must be unique, \"%s\" already declared\n", id->value.sval);
                                    sem_error(id, error_msg_symb);
                                }
                                
                                Formal * to_add = (Formal *)calloc(1,sizeof(Formal));
                                to_add->formal = id_node;
                                
                                if(last_formal==NULL){
                                    (func->aux)->formal=to_add;
                                }
                                else {
                                    last_formal->next=to_add;
                                }
                                last_formal = to_add;
                                
                                (func->aux)->formals_num++;//on single ID
                                id = id->brother;
                            }
                            child = child->brother; //next DECL
                        }
                    }
                    func->aux->num_obj = func->aux->formals_num;
                    
                    current = current->brother; //DOMAIN
                    func->schema = create_schema(current, func->father, NULL);
                    
                    current = current->brother; //TYPE_SECT_OPT
                    int not_used = 0;
                    handle_function_part(current, func, CLTYPE, &not_used);
                    
                    current = current->brother; //VAR_SECT_OPT
                    handle_function_part(current, func, CLVAR, &(func->aux->num_obj));
                    
                    current = current->brother; //CONST_SECT_OPT
                    handle_function_part(current, func, CLCONST, &(func->aux->num_obj));
                    
#if TABLE
                    print_func_node(func);
                    print_sch(func->schema);
                    print_hash_content((func->aux)->locenv);
#endif
                    current = current->brother; //FUNC_LIST_OPT
                    if (current->child != NULL) {
                        child = current->child; // FUNC DECL
                        while (child != NULL) {//loop on FUNC DECL
                            if (!insert_func(create_symbol_table(child, func), func)) {
                                sprintf(error_msg_symb, "Function names must be unique, \"%s\" already declared\n", child->child->value.sval);
                                sem_error(child->child, error_msg_symb);
                            }
                            child = child->brother;
                        }
                    }
                    return func;
                    break;
                default:
                    printf("ERROR: create_symbol_table called on wrong nonterminal\n");
                    break;
            }
            break;
        default:
            printf("ERROR: create_symbol_table called on a terminal\n");
            break;
    }
    printf("ERROR: something wrong occurred in create_symbol_table\n");
    return NULL;
}

Phash_node new_function_node(char * _name){
    Phash_node node = (Phash_node) calloc (1,sizeof(Hash_node));
    node->aux = calloc(1, sizeof(Aux_fun_param));
    node->name = _name;
    node->oid = oid;
    oid++;
    node->class_node = CLFUNC;
    return node;
}

Phash_node new_id_node(char * _name, Class _class, int loc_oid){
    Phash_node node = (Phash_node) calloc (1,sizeof(Hash_node));
    node->name = _name;
    node->oid = loc_oid;
    node->class_node = _class;
    return node;
}

Pschema create_schema(Pnode domain, Phash_node func, char * id){
    //func: function node of the local environment
    Pnode dom_child = domain->child;
    Pschema node = NULL;
    Pschema node_tmp = NULL;
    Phash_node type_decl = NULL;
    switch (dom_child->type) {
        case T_NONTERMINAL:
            switch (dom_child->value.ival) {
                case NSTRUCT_DOMAIN:
                    node = new_schema_node(SCSTRUCT);
                    node->id = id;
                    Pnode decl = dom_child->child;
                    
                    Pschema last = node->p1;
                    while (decl != NULL) { //DECL
                        Pnode decl_domain = decl->child->brother;
                        Pnode id = decl->child->child;
                        
                        
                        while (id != NULL) {
                            Pschema to_add = create_schema(decl_domain, func, id->value.sval);
                            if(last == NULL){
                                node->p1 = to_add;
                            }
                            else{
                                Pschema temp = node->p1;
                                while (temp != NULL){
                                    if (strcmp(to_add->id, temp->id)==0){
                                        sprintf(error_msg_symb, "Attributes in STRUCT decl must be unique, \"%s\" already present\n",to_add->id);
                                        sem_error(id,error_msg_symb);
                                    }
                                    temp = temp->p2;
                                }
                                
                                last->p2 = to_add;
                            }
                            last = to_add;
                            id = id->brother;
                        }
                        decl = decl->brother;
                    }
                    break;
                case NVECTOR_DOMAIN:
                    node = new_schema_node(SCVECTOR);
                    node->id = id;
                    node->size = dom_child->child->value.ival;
                    node->p1 = create_schema(dom_child->child->brother, func, NULL);
                    break;
                default:
                    break;
            }
            break;
        case T_ID:
            while (func != NULL) {
                type_decl = getNode(dom_child->value.sval, (func->aux)->locenv);
                if (type_decl != NULL){
                    node_tmp = type_decl->schema;
                    break;
                }
                func = func->father;
            }
            if (type_decl == NULL){
                sprintf(error_msg_symb, "ERROR type not found: %s\n", dom_child->value.sval);
                sem_error(dom_child, error_msg_symb);
            }
            node = new_schema_node(-1);
            memcpy(node, node_tmp, sizeof(Schema));
            node->id = id;
            break;
        case T_ATOMIC_DOMAIN:
            //CHAR, INT, REAL, STRING, BOOL
            switch (dom_child->value.ival) {
                case CHAR:
                    node = new_schema_node(SCCHAR);
                    break;
                case INT:
                    node = new_schema_node(SCINT);
                    break;
                case REAL:
                    node = new_schema_node(SCREAL);
                    break;
                case STRING:
                    node = new_schema_node(SCSTRING);
                    break;
                case BOOL:
                    node = new_schema_node(SCBOOL);
                    break;
                default:
                    break;
            }
            node->id = id;
            break;
        default:
            break;
    }
    return node;
}
