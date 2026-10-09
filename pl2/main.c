#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "parce.h"
#include "backtracking.h"

int main() {
	
	int idx = 0;
	int first_idx = 0;
	int digits[26] ;
	memset(digits, -1, sizeof(digits));
	
	
	
	char uniq_letters[10] = {0};
	char first_letters[8] = {0};
	int busy_digits[10];
	memset(busy_digits, 0, sizeof(busy_digits));
	int uniq_size;
	
	char* str = get_string();
	int j=0;
	
	
	if (!str) {
		return 1;
	}
	char** arr = malloc(8 * sizeof(char*));
	if (arr == NULL) {
		return 1;
	}

	
	char* token = strtok(str, "+");

	while (token != NULL) {
		arr[idx] = malloc(strlen(token) + 1);
		
		if (!arr[idx]) {
			for (int i = 0; i < idx; i++) {
				free(arr[i]);
			}
			return 1;
		}

		strcpy(arr[idx], token);
		if (!in_string(first_letters, arr[idx][0],8)) { first_letters[first_idx] = arr[idx][0]; first_idx++; }
		
		//printf("%s\n", arr[idx]);
		idx++;
		token = strtok(NULL, "+");

	}
	
	char* result = malloc(strlen(arr[idx - 1]) + 1);
	strcpy(result, arr[idx - 1]);
	int* summands = malloc(idx * sizeof(int));
	uniq_size=get_uniq_letters(arr, idx, uniq_letters);
	int success=backtracking(0, uniq_letters, uniq_size, arr, idx, digits, busy_digits, first_letters, summands);
	if (!success) {
		printf("NO SOLUTIONS");
	}

	





	free(summands);
	free(result);
	for (int i = 0; i < idx; i++) {

		free(arr[i]);
	}

	free(arr);
	free(str);

	

	return 0;
}