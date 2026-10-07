#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
char* get_string() {
	int ch;
	int size = 2;
	int cap = 0;
	char* str = malloc(size * sizeof(char));
	if (str == NULL) {
		return NULL;
	}

	while ((ch = getc(stdin)) != '\n') {
		if (cap >= size) {
			size *= 2;
			str = realloc(str, size);
			if (str == NULL) {
				return NULL;
			}
		}
		if (isalpha(ch) || ch == '+' || ch == '=') {
			if (ch == '=') { ch = '+'; }
			str[cap] = ch;
			cap++;
		}
		
	}
	if (cap >= size) {
		size +=1;
		str = realloc(str, size);
		if (str == NULL) {
			return NULL;
		}
	}
	str[cap] = '\0';
	
	return str;
}


int main() {
	char* str = get_string();
	char** arr = malloc(8 * sizeof(char*));
	if (arr == NULL) {
		return 1;
	}
	//printf("%s", str);
	int idx = 0;
	char* token = strtok(str, "+");
	
	while (token != NULL) {
		arr[idx] = malloc(strlen(token)+1);
		if (!arr[idx]) { return 1; }
		
		strcpy(arr[idx], token);
		
		printf("%s\n", arr[idx]);
		idx++;
		token = strtok(NULL, "+");
		
	}
	char* result=malloc(strlen(arr[idx-1])+1);
	strcpy(result, arr[idx-1]);
	printf("%s", result);
	

	free(result);
	for (int i = 0; i < idx; i++) {

		free(arr[i]);
	}

	free(arr);
	free(str);



	return 0;
}
