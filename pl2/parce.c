#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool in_string(char* arr,char letter,int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == letter) {
			return true;
		}
	}
	return false;
}

char* get_string() {
	int ch;
	int size = 2;
	int cap = 0;
	char* str = malloc(size * sizeof(char));
	if (str == NULL) {
		return NULL;
	}

	while ((ch = getc(stdin)) != '\n' && ch!=EOF) {
		if (cap >= size) {
			size *= 2;
			str = realloc(str, size);
			if (str == NULL) {
				return NULL;
			}
		}
		if (isalpha(ch) || ch != ' ') {
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

int get_uniq_letters(char** arr,int size, char* uniq_letters) {
	int idx = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < strlen(arr[i]); j++) {
			if (in_string(uniq_letters,arr[i][j],10)) {
				continue;
			}
			else {
				uniq_letters[idx] = arr[i][j];
				idx++;
			}
		}
	}
	if (idx <= 10) {
		return idx;
	}
	printf("uniq letters > 10 ");
	exit(1);

}



