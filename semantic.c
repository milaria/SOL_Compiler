#include "semantic.h"

char error_msg[100];


int program(Pnode root, Phash_node loc_env){
	return func_decl(root->child, loc_env);
}
int func_decl(Pnode root, Phash_node loc_env){
	Pnode id = root->child;
	Pnode current = id->brother;
    Pschema stype;
    
	int decl_list_opt_ok = decl_list_opt(current);
	current = current->brother;
	int domain_ok = domain(current, stype);
	current = current->brother;
	int type_sect_opt_ok = type_sect_opt(current);
	current = current->brother;
	int var_sect_opt_ok = var_sect_opt(current);
	current = current->brother;
	int const_sect_opt_ok = const_sect_opt(current);
	current = current->brother;
	int func_list_opt_ok = func_list_opt(current);
	current = current->brother;
	int func_body_ok = func_body(current);

}
int decl_list_opt(Pnode root, Phash_node loc_env){
	Pnode current = root->child;
	int decl_list_opt_ok = 1;
	while(current != NULL){
		decl_list_opt_ok = decl_list_opt_ok && decl(current);
		current = current->brother;
	}
	return decl_list_opt_ok;
}
int decl(Pnode root, Phash_node loc_env){
    
}
int id_list(Pnode root, Phash_node loc_env){
    
}
int domain(Pnode root, Phash_node loc_env, Pschema stype){
    
}
int struct_domain(Pnode root, Phash_node loc_env){
    
}
int vector_domain(Pnode root, Phash_node loc_env){
    
}
int type_sect_opt(Pnode root, Phash_node loc_env){
    
}
int var_sect_opt(Pnode root, Phash_node loc_env){
    
}
int const_sect_opt(Pnode root, Phash_node loc_env){
    
}
int func_list_opt(Pnode root, Phash_node loc_env){
    
}
int func_body(Pnode root, Phash_node loc_env){
    
}
int stat_list(Pnode root, Phash_node loc_env){
    
}
int stat(Pnode root, Phash_node loc_env){
    
}
int assign_stat(Pnode root, Phash_node loc_env){
    
}
int left_hand_side(Pnode root, Phash_node loc_env, Pschema type){
    int lhs_ok;
    switch (root->type) {
        case T_ID:
            
            break;
        case T_NONTERMINAL:
            switch (root->value.ival) {
                case NFIELDING:
                    lhs_ok = fielding(root->child, type);//TODO
                    break;
                case NINDEXING:
                    lhs_ok = indexing(root->child, type);//TODO
                    break;
                default:
                    semantic_error("Some weird nonterminal node in lhs\n");
                    break;
            }
            break;
        default:
            semantic_error("Some weird terminal node in lhs\n");
            break;
    }
}
int fielding(Pnode root, Phash_node loc_env){
    
}
int indexing(Pnode root, Phash_node loc_env){
    
}
int if_stat(Pnode root, Phash_node loc_env){
    
}
int elsif_stat_list_opt(Pnode root, Phash_node loc_env){
    
}
int while_stat(Pnode root, Phash_node loc_env){
    
}
int for_stat(Pnode root, Phash_node loc_env){
    
}
int foreach_stat(Pnode root, Phash_node loc_env){
    
}
int return_stat(Pnode root, Phash_node loc_env){
    
}
int read_stat(Pnode root, Phash_node loc_env){
    
}
int specifier_opt(Pnode specifier_opt){ // NULL or STRING
    Pnode specifier = specifier_opt->child;
    Pschema type_spec = new_schema_node(-1);
    int ok;
    int spec_ok = (specifier == NULL);
    if (!spec_ok) {
        ok = expr(specifier->child, type_spec);
        spec_ok = (type_spec->type == STRING);
    }
    if (!spec_ok) {
        semantic_error("Type error, specifier in wr/write/rd/read call must be a STRING or NULL");
    }
    return ok && spec_ok;
}

int write_stat(Pnode root, Phash_node loc_env){
    
}
int math_expr(Pnode root, Phash_node loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	
	int expr1_ok = expr(expr1, expr1_type);
	if(expr1_type->type != INT || expr1_type->type != REAL){
		//sprintf(error_msg,"Type error, expected INT | REAL instead %s \n", tabsem_types[expr1_type]);
		sprintf(error_msg,"Type error, expected INT | REAL instead %s \n", "to_do");
		semantic_error(error_msg);
	}
	int expr2_ok = expr(expr2, expr2_type);
	if(expr2_type->type != expr1_type->type){
		//sprintf(error_msg,"Type mismatch, expected %s instead %s\n", tabsem_types[expr1_type],tabsem_types[expr2_type]);
		sprintf(error_msg,"Type mismatch, expected %s instead %s\n", "to_do", "to_do");
		semantic_error(error_msg);
	}
	stype->type = expr1_type->type;
	return expr1_ok && expr2_ok;
}
int logic_expr(Pnode root, Phash_node loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
    
	int expr1_ok = expr(expr1, expr1_type);
	if(expr1_type->type != BOOL)
		semantic_error("Type error, expected BOOL\n");
	int expr2_ok = expr(expr2, expr2_type);
	if(expr2_type->type != BOOL)
		semantic_error("Type error, expected BOOL\n");
	stype->type = BOOL;
	return expr1_ok && expr2_ok;
}
int rel_expr(Pnode root, Phash_node loc_env, Pschema stype){
	Pnode expr1 = root->child;
	Pnode expr2 = root->child->brother;
	Pschema expr1_type = new_schema_node(-1);
	Pschema expr2_type = new_schema_node(-1);
	int expr1_ok = expr(expr1, expr1_type);
	int expr2_ok = expr(expr2, expr2_type);
	int type_ok;
	switch(root->qualifier){
		case EQ:
		case NE:
			type_ok = expr1->type == expr2->type;
            if(!type_ok)
                semantic_error("Type mismatch in relational expression\n");
			break;
		case '>':
		case GE:
		case '<':
		case LE:
            type_ok = (expr1_type->type == INT || expr1_type->type == CHAR || expr1_type->type == REAL || expr1_type->type == STRING);
            if (!type_ok) {
                semantic_error("Type error in relational expression, expected INT, CHAR, REAL or STRING\n");
            }
            type_ok = type_ok && (expr1_type->type == expr2_type->type);
            if(!type_ok)
                semantic_error("Type mismatch in relational expression\n");
			break;
		case IN:
            if (expr2_type->type == VECTOR) {
                type_ok = are_compatible(expr1_type, expr2_type->p1 );
            }
			break;
		default:
			semantic_error("Some weird qualification in relational expression\n");
	}
    
	stype->type = BOOL;
	return expr1_ok && expr2_ok && type_ok;
}
int neg_expr(Pnode root, Phash_node loc_env, Pschema stype){
	Pschema expr_type = new_schema_node(-1);
	int expr_ok = expr(root->child, expr_type);
	switch(root->qualifier){
		case '-':
			if(expr_type->type != INT || expr_type->type != REAL){
				//sprintf(error_msg,"Type error, expected INT | REAL instead of %s \n", tabsem_types[expr_type]);//
				sprintf(error_msg,"Type error, expected INT | REAL instead of %s \n", "to_do");
				semantic_error(error_msg);
			}
			stype->type = expr_type->type;
            break;
		case NOT:
			if(expr_type->type != BOOL){
				//sprintf(error_msg,"Type error, expected BOOL instead of %s \n", tabsem_types[expr_type]);//
				sprintf(error_msg,"Type error, expected BOOL instead of %s \n", "to_do");
				semantic_error(error_msg);
			}
			stype->type = BOOL;
            break;
	}
	return expr_ok;
}
int wr_expr(Pnode root, Phash_node loc_env, Pschema stype){
    int ok = specifier_opt(root->child);
    int expr_ok;
    if (ok) {
        expr_ok = expr(root->child->brother, stype);
    }
    return ok && expr_ok;
}
int rd_expr(Pnode root, Phash_node loc_env, Pschema stype){
    int ok = specifier_opt(root->child);
    int dom_ok;
    if (ok) {
        dom_ok = domain(root->child->brother, stype);
    }
    return ok && dom_ok;
}
int instance_expr(Pnode root, Phash_node loc_env, Pschema stype){
	int expr_ok;
	int count = 0;
	Pschema current_schema = new_schema_node(-1); //allocate schema
	Pnode current_node;
	switch(root->qualifier){
		case STRUCT:
			stype->type = STRUCT;
			
			current_node = root->child; //first element of struct
			
			expr_ok = expr(current_node, current_schema); //eval first child's schema
			stype->p1 = current_schema; // attach to root's schema the schema of first child
			current_node = current_node->brother; //switch to the firse brother

			while (current_node){//cicle for the other brother
				Pschema next = new_schema_node(-1);
				expr_ok = expr(current_node, next);
				current_schema->p2 = next;
				current_schema = next;
				current_node = current_node->brother;
			}
		break;
		case VECTOR:
			stype->type = VECTOR;
			
			current_node = root->child;
			
			expr_ok = expr(current_node, current_schema);
			current_node = current_node->brother;
			count++;
			while (current_node){
				Pschema next = new_schema_node(-1);
				expr_ok = expr_ok && expr(current_node, next);
				if(!are_compatible(next,current_schema)){
					semantic_error("vector type error");
					break;
				}
				current_node = current_node->brother;
				count++;
			}
			stype->size = count;
			stype->p1 = current_schema;
		break;
	}
	return expr_ok;
}
int func_call(Pnode root, Phash_node loc_env, Pschema stype){
    
}
int cond_expr(Pnode root, Phash_node loc_env, Pschema stype){
	Pschema expr1_type = new_schema_node(-1);
	int expr1_ok = expr(root->child, expr2_type);
	if (expr1_type->type!=BOOL){
		semantic_error("error");
	}
    
}
int elsif_expr_list_opt(Pnode root, Phash_node loc_env){
    
}
int built_in_call(Pnode root, Phash_node loc_env, Pschema stype){
    
}

int expr(Pnode root, Phash_node loc_env, Pschema stype){
	int expr_ok;
    
	switch(root->type){
		case T_CHARCONST:
			stype->type = CHAR;
			break;
		case T_INTCONST:
			stype->type = INT;
			break;
		case T_REALCONST:
			stype->type = REAL;
			break;
		case T_STRCONST:
			stype->type = STRING;
			break;
		case T_BOOLCONST:
			stype->type = BOOL;
			break;
		case T_NONTERMINAL:
            switch(root->value.ival){
                case NLEFT_HAND_SIDE:
                    expr_ok = left_hand_side(root, stype);
                    break;
                case NMATH_EXPR:
                    expr_ok = math_expr(root, stype);
                    break;
                case NLOGIC_EXPR:
                    expr_ok = logic_expr(root, stype);
                    break;
                case NREL_EXPR:
                    expr_ok = rel_expr(root, stype);
                    break;
                case NNEG_EXPR:
                    expr_ok = neg_expr(root, stype);
                    break;
                case NWR_EXPR:
                    expr_ok = wr_expr(root, stype);
                    break;
                case NRD_EXPR:
                    expr_ok = rd_expr(root, stype);
                    break;
                case NINSTANCE_EXPR:
                    expr_ok = instance_expr(root, stype);
                    break;
                case NFUNC_CALL:
                    expr_ok = func_call(root, stype);
                    break;
                case NCOND_EXPR:
                    expr_ok = cond_expr(root, stype);
                    break;
                case NBUILT_IN_CALL:
                    expr_ok = built_in_call(root, stype);
                    break;
                default:
                    semantic_error("Some weird nonterminal node in expr\n");
            }
            break;
		default:
			semantic_error("Some weird terminal node in expr\n");
	}
	return expr_ok;
}

void semantic_error(char * msg ){
    fprintf(stderr, "Semantic error: %s", msg);
    exit(EXIT_FAILURE);
}