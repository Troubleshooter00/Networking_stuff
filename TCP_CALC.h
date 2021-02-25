#ifndef TCP_CALC
#define TCP_CALC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

enum operation_type{
	TYPE_ADD,
	TYPE_SUB,
	TYPE_MUL,
	TYPE_DIV
};

struct operation{
	enum operation_type type;
	long left;
	long right;
};

int parse_string(const char* input, struct operation* output){
	long left, right;
	enum operation_type type;
	const char* p= input;
	char* end;

	left = strtol(p, &end, 10);
	if(p == end){
		return -1;
	}
	p = end;

	while(isspace((unsigned char)*p)){
		p++;
	}
	switch(*p){
		case '+':
			type = TYPE_ADD;
			break;
		case '-':
			type = TYPE_SUB;
			break;
		case '*':
			type = TYPE_MUL;
			break;
		case '/':
			type = TYPE_DIV;
			break;
		default:
			return -1;
	}
	p++;

	errno = 0;
	right = strtol(p, &end, 10);
	if(p == end || errno == ERANGE){
		return -1;
	}
	p = end;
	while(isspace((unsigned char)*p)){
		p++;
	}
	if(*p != '\0'){
		return -1;
	}
	output->type = type;
	output->left = left;
	output->right = right;

	return 0;
}

#endif
