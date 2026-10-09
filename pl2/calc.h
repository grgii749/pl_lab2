#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>




typedef enum {
    token_digit_d,
    token_operator,
    token_function,
    token_variable,
    token_openbracket,
    token_endbracket,
    token_unominus
} token_type;

typedef enum {
    ptiority_bracket,
    priotity_sumdiff,
    priority_multdiv,
    priotity_pow,
    priority_unfunc
} priority;

typedef enum {
    ass_left,
    ass_right,
    ass_no
} ass;

typedef struct {
    priority prty;
    ass as;
    token_type type;
    union {
        double d_num;
        char* name;
        char oper;
    } val;
} token;


typedef struct node_queue {
    token data;
    struct node_queue* next;
} node_queue;

typedef struct {
    node_queue* front;
    node_queue* rear;
} queue;


typedef struct node_stack {
    token data;
    struct node_stack* next;
} node_stack;

typedef struct {
    node_stack* top;
} stack;


token copy_token(token src);

void  init_queue(queue* q);
int   check_empty_q(queue* q);
void  add_queue(queue* q, token t);
token delete_q(queue* q);
void  clear_queue(queue* q);

void  init_stack(stack* s);
int   check_empty_s(stack* s);
void  push(stack* s, token t);
token pop(stack* s);
token peek_stack(stack* s);

int    is_oper(char c);
double fact(double x);



void tokenize(char* input, queue* q);
void postfix(queue* q_inp, queue* q_out, stack* s);
void eval(queue* q, stack* s, double* result, char* c);


