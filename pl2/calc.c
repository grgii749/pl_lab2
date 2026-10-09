#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#define pi 3.14159265
#define max_len 1000
#define len_str 256
typedef enum {
	token_digit_d,
	token_operator,
	token_function,
	token_variable,
	token_openbracket,
	token_endbracket,
	token_unominus
}token_type;
typedef enum {
	ptiority_bracket,
	priotity_sumdiff,
	priority_multdiv,
	priotity_pow,
	priority_unfunc
}priority;
typedef enum {
	ass_left,
	ass_right,
	ass_no
}ass;
typedef struct {
	priority prty;
	ass as;
	token_type type;
	union {
		double d_num;
		char* name;
		char oper;
	}val;
} token;
typedef struct node_queue {
	token data;
	struct node_queue* next;
}node_queue;
typedef struct queue {
	node_queue* front;
	node_queue* rear;
}queue;
typedef struct node_stack {
	token data;
	struct node_stack* next;
}node_stack;
typedef struct {
	node_stack* top;
}stack;
token copy_token(token src) {
	token dest = src;
	if (src.type == token_function || src.type == token_variable) {
		dest.val.name = malloc(strlen(src.val.name) + 1);
		if (dest.val.name == NULL) {
			return;
		}
		strcpy(dest.val.name, src.val.name);
	}
	return dest;
}

void init_queue(queue* q) {
	q->front = NULL;
	q->rear = NULL;
}
int check_empty_q(queue* q) {
	return (q->front == NULL);
}
void add_queue(queue* q, token t) {
	node_queue* new_node_q = malloc(sizeof(node_queue));
	if (new_node_q == NULL) {
		return;
	}
	new_node_q->data = t;
	new_node_q->next = NULL;
	if (check_empty_q(q)) {
		q->front = new_node_q;
		q->rear = new_node_q;
	}
	else {
		q->rear->next = new_node_q;
		q->rear = new_node_q;
	}
}
token delete_q(queue* q) {
	node_queue* cur = q->front;
	token res = cur->data;
	q->front = q->front->next;
	if (q->front == NULL) {
		q->rear = NULL;
	}
	if (res.type == token_function || res.type == token_variable) {
		free(res.val.name);
	}
	free(cur);
	return res;
}
void clear_queue(queue* q) {
	while (!check_empty_q(q)) {
		token t = delete_q(q);
	}
}

void init_stack(stack* s) {
	s->top = NULL;
}
int check_empty_s(stack* s) {
	return s->top == NULL;
}
void push(stack* s, token t) {
	node_stack* new_node_stack = (node_stack*)malloc(sizeof(node_stack));
	if (new_node_stack == NULL) {
		return;
	}
	new_node_stack->data = t;
	new_node_stack->next = s->top;
	s->top = new_node_stack;
}
token pop(stack* s) {
	if (check_empty_s(s)) {
		exit(1);
	}
	node_stack* temp = s->top;
	token res = temp->data;
	s->top = temp->next;
	free(temp);
	return res;
}
token peek_stack(stack* s) {
	if (check_empty_s(s)) {
		token empt = { 0 };
		return empt;
	}
	return s->top->data;
}
int is_oper(char c) {
	return c == '+' ||
		c == '-' ||
		c == '^' ||
		c == '/' ||
		c == '*' ||
		c == '!';
}
double fact(double x) {
	if (x == 0) { return 1; }
	if (x == 1) { return 1; }
	return (x * fact(x - 1));
}
void  tokenize(char* input, queue* q) {
	init_queue(q);
	int len = strlen(input);
	int i = 0;
	while (i < len) {
		if (isspace(input[i])) { i++; continue; }
		token t;
		if (isdigit(input[i]) || input[i] == '.') {
			char digit[len_str];
			int j = 0;
			digit[j] = input[i];
			j++;
			i++;
			while (i < len && j < len_str && (isdigit(input[i]) || input[i] == '.')) {
				digit[j] = input[i];
				i++; j++;
			}
			digit[j] = '\0';
			t.type = token_digit_d;
			t.val.d_num = atof(digit);
			add_queue(q, t);
			continue;
		}
		else if (is_oper(input[i])) {
			if (i == 0 || (i >= 1 && input[i - 1] == '(')) {
				t.prty = priority_unfunc;
				t.as = ass_right;
				t.type = token_unominus;
				t.val.oper = '~';
				add_queue(q, t);
				i++;
				continue;
			}
			else {
				if (input[i] == '+' || input[i] == '-') {
					t.prty = priotity_sumdiff;
					t.as = ass_left;
				}
				else if (input[i] == '*' || input[i] == '/') {
					t.prty = priority_multdiv;
					t.as = ass_left;
				}
				else if (input[i] == '!') {
					t.prty = priority_unfunc;
					t.as = ass_left;
				}
				else if (input[i] == '^') {
					t.prty = priotity_pow;
					t.as = ass_right;
				}
				t.type = token_operator;
				t.val.oper = input[i];
				add_queue(q, t);
				i++;
				continue;
			}
		}
		else if (isalpha(input[i])) {
			int k = 0;
			char sym[len_str];
			sym[k] = input[i];
			k++; i++;
			while ((k < len) && (k < len_str) && (isalpha(input[i]))) {
				sym[k] = input[i];
				k++; i++;
			}
			sym[k] = '\0';
			if (strcmp(sym, "sin") == 0 || strcmp(sym, "cos") == 0 || strcmp(sym, "tg") == 0 ||
				strcmp(sym, "ctg") == 0 || strcmp(sym, "arcsin") == 0 || strcmp(sym, "arccos") == 0 ||
				strcmp(sym, "arctg") == 0 || strcmp(sym, "arcctg") == 0 || strcmp(sym, "sqrt") == 0) {
				t.type = token_function;
				t.as = ass_left;
				t.prty = priority_unfunc;
			}
			else {
				t.type = token_variable;
			}
			t.val.name = malloc((strlen(sym) + 1) * sizeof(char));
			if (t.val.name == NULL) {
				clear_queue(q);
				return;
			}
			strcpy(t.val.name, sym);
			add_queue(q, t);
			continue;
		}
		else if (input[i] == '(') {
			if (input[i + 1] == ')') {
				t.type = token_digit_d; t.val.d_num = 0; add_queue(q, t); i += 2; continue;
			}
			else {
				t.type = token_openbracket;
				i++;
				add_queue(q, t);
				continue;
			}
		}
		else if (input[i] == ')') {
			t.type = token_endbracket;
			i++;
			add_queue(q, t);
			continue;
		}
	}
}
void postfix(queue* q_inp, queue* q_out, stack* s) {
	node_queue* cur = q_inp->front;
	while (cur != NULL) {
		token t = cur->data;
		if (t.type == token_digit_d || t.type == token_variable) {
			token copy = copy_token(t);
			add_queue(q_out, copy);
		}
		else if (t.type == token_operator || t.type == token_unominus) {
			if (check_empty_s(s)) { push(s, copy_token(t)); }
			else if (t.prty <= peek_stack(s).prty) {
				while (!check_empty_s(s) && peek_stack(s).type != token_openbracket &&
					(peek_stack(s).prty > t.prty || (peek_stack(s).prty == t.prty && t.as == ass_left))) {
					token del = pop(s);
					token copy = copy_token(del);
					add_queue(q_out, copy);
				}
				push(s, copy_token(t));
			}
			else if (t.prty > peek_stack(s).prty) {
				push(s, copy_token(t));
			}
		}
		else if (t.type == token_openbracket) {
			push(s, t);
		}
		else if (t.type == token_endbracket) {
			while (!check_empty_s(s) && peek_stack(s).type != token_openbracket) {
				token delb = pop(s);
				token copy = copy_token(delb);
				add_queue(q_out, copy);
			}
			if (!check_empty_s(s) && peek_stack(s).type == token_openbracket) {
				pop(s);
			}
			if (!check_empty_s(s) && peek_stack(s).type == token_function) {
				token func = pop(s);
				token copy = copy_token(func);
				add_queue(q_out, copy);
			}
		}
		else if (t.type == token_function) {
			push(s, copy_token(t));
		}
		cur = cur->next;
	}
	while (!check_empty_s(s)) {
		token lasts = pop(s);
		add_queue(q_out, lasts);
	}
}
void eval(queue* q, stack* s, double* result, char* c) {
	*c = 0;
	node_queue* cur = q->front;
	if (cur == NULL) {
		return;
	}
	*result = 0;
	while (cur != NULL) {
		token t = cur->data;
		if (t.type == token_digit_d) {
			push(s, t);
		}
		else if (t.type == token_variable) {
			double users_var;
			printf("Enter %s = ", t.val.name);
			scanf("%lf", &users_var);
			t.type = token_digit_d;
			t.val.d_num = users_var;
			push(s, t);
		}
		else if (t.type == token_operator) {
			if (t.val.oper == '+') {
				token t1 = pop(s);
				token t2 = pop(s);
				double cur_res = 0;
				cur_res = t2.val.d_num + t1.val.d_num;
				token res;
				res.val.d_num = cur_res;
				push(s, res);
			}
			if (t.val.oper == '-') {
				token t1 = pop(s);
				token t2 = pop(s);
				double cur_res = 0;
				cur_res = t2.val.d_num - t1.val.d_num;
				token res;
				res.val.d_num = cur_res;
				push(s, res);
			}
			if (t.val.oper == '*') {
				token t1 = pop(s);
				token t2 = pop(s);
				double cur_res = 0;
				cur_res = t2.val.d_num * t1.val.d_num;
				token res;
				res.val.d_num = cur_res;
				push(s, res);
			}
			if (t.val.oper == '/') {
				token t1 = pop(s);
				token t2 = pop(s);
				double cur_res = 0;
				if (t1.val.d_num == 0) {
					printf("Error! Division by zero\n");
					c = 1;
					return;
				}
				cur_res = t2.val.d_num / t1.val.d_num;
				token res;
				res.val.d_num = cur_res;
				push(s, res);
			}
			if (t.val.oper == '^') {
				token t1 = pop(s);
				token t2 = pop(s);
				double cur_res = 0;
				cur_res = pow(t2.val.d_num, t1.val.d_num);
				token res;
				res.val.d_num = cur_res;
				push(s, res);
			}
			if (t.val.oper == '!') {
				token t1 = pop(s);
				if (t1.val.d_num < 0 || t1.val.d_num != (int)t1.val.d_num) {
					printf("The factorial is defined only for positive integers\n");
					c = 1;
					return;
				}
				token res;
				token factor;
				factor.val.d_num = fact(t1.val.d_num);
				double cur_res = factor.val.d_num;
				res.val.d_num = cur_res;
				push(s, res);
			}
		}
		else if (t.type == token_unominus) {
			token t1 = pop(s);
			double cur_res = 0;
			cur_res = 0 - t1.val.d_num;
			token res;
			res.val.d_num = cur_res;
			push(s, res);
		}
		else if (t.type == token_function) {
			token t1 = pop(s);
			double arg = t1.val.d_num;
			double cur_res = 0;
			if (strcmp(t.val.name, "sin") == 0) { cur_res = sin(arg); }
			else if (strcmp(t.val.name, "cos") == 0) { cur_res = cos(arg); }
			else if (strcmp(t.val.name, "tg") == 0) { cur_res = tan(arg); }
			else if (strcmp(t.val.name, "ctg") == 0) { cur_res = 1 / tan(arg); }
			else if (strcmp(t.val.name, "arcsin") == 0) { cur_res = asin(arg); }
			else if (strcmp(t.val.name, "arccos") == 0) { cur_res = acos(arg); }
			else if (strcmp(t.val.name, "arctg") == 0) { cur_res = atan(arg); }
			else if (strcmp(t.val.name, "arcctg") == 0) { cur_res = atan(-arg) + pi / 2; }
			else if (strcmp(t.val.name, "sqrt") == 0) { cur_res = sqrt(arg); }
			token res;
			res.type = token_digit_d;
			res.val.d_num = cur_res;
			push(s, res);
		}
		cur = cur->next;
	}
	if (!check_empty_s(s)) {
		token res_token = pop(s);
		*result = res_token.val.d_num;
	}
}
double evaluate(const char* expr) {
	queue q, qo;
	stack s, s1;
	double res = 0;
	char err = 0;

	init_queue(&q);
	init_queue(&qo);
	init_stack(&s);
	init_stack(&s1);

	char buf[max_len];
	strncpy(buf, expr, max_len - 1);
	buf[max_len - 1] = '\0';

	tokenize(buf, &q);
	postfix(&q, &qo, &s);
	eval(&qo, &s1, &res, &err);

	clear_queue(&q);
	clear_queue(&qo);
	return  res;
}