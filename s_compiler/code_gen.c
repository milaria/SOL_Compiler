#include "code_gen.h"
//#include "parser.h"

char * tabOperator[]={
    "NEW",
    "NEWS",
    "LDC",
    "LDI",
    "LDR",
    "LDS",
    "LOD",
    "CAT",
    "LDA",
    "FDA",
    "EIL",
    "SIL",
    "IXA",
    "STO",
    "IST",
    "JMF",
    "JMP",
    "EQU",
    "NEQ",
    "CGT",
    "IGT",
    "RGT",
    "SGT",
    "CGE",
    "IGE",
    "RGE",
    "SGE",
    "CLT",
    "ILT",
    "RLT",
    "SLT",
    "CLE",
    "ILE",
    "RLE",
    "SLE",
    "IN",
    "IPLUS",
    "RPLUS",
    "IMINUS",
    "RMINUS",
    "ITIMES",
    "RTIMES",
    "IDIV",
    "RDIV",
    "IUMI",
    "RUMI",
    "NEG",
    "WR",
    "FWR",
    "PUSH",
    "GOTO",
    "POP",
    "RD",
    "FRD",
    "TOINT",
    "TOREAL",
    "READ",
    "FREAD",
    "WRITE",
    "FWRITE",
    "FUNC",
    "HALT",
    "SCODE"};

char convert_bool[] = {'0','1'};

void relocate_address(Code code, int offset){
    Stat *pt = code.head; int i;
    for(i = 1; i <= code.size; i++){
        pt->address += offset;
        pt = pt->next;
    }
}

Code appcode(Code code1, Code code2){
    Code rescode;
    relocate_address(code2, code1.size);
    rescode.head = code1.head;
    rescode.tail = code2.tail;
    code1.tail->next = code2.head;
    rescode.size = code1.size + code2.size;
    return rescode;
}

Code endcode(){
    static Code code = {NULL, 0, NULL};
    return code;
}

Code concode(Code code1, Code code2, ...){
    Code rescode = code1, *pcode = &code2;
    while(pcode->head != NULL){
        rescode = appcode(rescode, *pcode);
        pcode++;
    }
    return rescode;
}

Stat * newstat(Operator op){
    Stat *pstat;
    pstat = (Stat*) malloc(sizeof(Stat));
    if (!pstat) {
        fprintf(stderr, "Impossible to allocate new memory\n");
        exit(EXIT_FAILURE);
    }
    pstat->address = 0;
    pstat->op = op;
    pstat->next = NULL;
    return pstat;
}

Code makecode(Operator op){
    Code code;
    code.head = code.tail = newstat(op);
    code.size = 1;
    return code;
}

Code makecode1(Operator op, int arg){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg;
    return code;
}

Code makecode2(Operator op, int arg1, int arg2){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    return code;
}

Code makecode3(Operator op, int arg1, int arg2, int arg3){
    Code code;
    code = makecode(op);
    code.head->args[0].ival = arg1;
    code.head->args[1].ival = arg2;
    code.head->args[2].ival = arg3;
    return code;
}

Code make_push_pop(int size, int chain, int entry){
    return concode(makecode2(S_PUSH, size, chain),
                   makecode1(S_GOTO, entry),
                   makecode(S_POP),
                   endcode());
}

Code make_ldc(char c){
    Code code;
    code = makecode(S_LDC);
    code.head->args[0].ival = c;
    return code;
}

Code make_ldi(int i){
    Code code;
    code = makecode(S_LDI);
    code.head->args[0].ival = i;
    return code;
}

Code make_ldr(float r){
    Code code;
    code = makecode(S_LDR);
    code.head->args[0].rval = r;
    return code;
}

Code make_lds(char *s){
    Code code;
    code = makecode(S_LDS);
    code.head->args[0].sval = s;
    return code;
}

void print_stat(Stat * stat, FILE * stream){
    fprintf(stream, "%s ", tabOperator[stat->op]);
    fprintf(stream, "\n");
    // if(stat->args[])
    // fprintf(stream, "%s ", tabOperator[stat->op]);
}

void print_code(Code * code, FILE * stream){
    Stat * stat;
    stat = code->head;
    while(stat){
        print_stat(stat, stream);
        stat = stat->next;
    }
}
