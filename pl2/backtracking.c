#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "parse.h"

void print_result(int* summands, int size) {
	printf("%d ", summands[0]);
	for (int i = 1; i < size-1; i++) {
		printf("+ ");
		printf("%d ", summands[i]);
	}
	printf("= ");
	printf("%d", summands[size - 1]);

}
int word_to_digit(char* word,int *digits) {
	int res = 0;
	int idx = 0;
	for (int i = 0; i < 26; i++) {
		if (idx < strlen(word)) {
			char letter = word[idx];
			if (digits[letter - 'A'] != -1) {
				idx++;
				res = res * 10 + digits[letter - 'A'];
			}
		}
	}
	return res;
}
int backtracking(int idx, char* uniq_letters, int uniq_size, char** words, int words_size, int* digits, int* busy_digits, char* first_letters,int *summands) {
	if (idx == uniq_size) {
		int result = 0;
		for (int i = 0; i < words_size-1; i++) {
			summands[i]= word_to_digit(words[i], digits);
			result += summands[i];
		}
		if (result == word_to_digit(words[words_size-1], digits)) {
			summands[words_size - 1] = result;
			print_result(summands, words_size);
			return 1;
		}
		return 0;
	}
	
	
	
	for (int i = 0; i < 10; i++) {
		char letter= uniq_letters[idx];
		
		if (in_string(first_letters, letter, 8) && i==0) { continue; }
		if (busy_digits[i]) { continue; }
		else {
			digits[letter - 'A'] = i;;
			busy_digits[i] = 1;
		}
		if (backtracking(idx + 1, uniq_letters, uniq_size, words, words_size, digits, busy_digits, first_letters,summands)) {
			return 1;
		}
		digits[letter - 'A'] = -1;
		busy_digits[i] = 0;
	}
	return 0;
}