#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
int word_to_digit(char* word,int *digits);
int backtracking(int idx, char* uniq_letters, int uniq_size, char** words, int words_size, int* digits, int* busy_digits, char* first_letters,int* summands,int * last_column);
void print_result(int* summands, int size);