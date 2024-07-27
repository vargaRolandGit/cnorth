/*

	!!! WARNING BAD C CODE !!!
	made by: Varga Roland
	https://github.com/vargaRolandGit/cnorth
	Version: BETA0.2

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char tokens[64][512];
int token_counter = 0;

int stack[128];
int stack_pointer = 0;

char* read_file(char* filename) {
	FILE* f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buffer = (char*)malloc(length + 1);
	buffer[length] = '\0';
	fread(buffer, 1, length, f);
	fclose(f);
	buffer[length] = '\0';
	return buffer;
}

void usage() {
	printf("USAGE: \n\tnorth <file_path>\n");
}

const char* whitespace = " \r\t,\n";
const char* delimiter  = " \r\t,\n";

typedef enum Error {
	ERROR_NONE = 0,
	ERROR_SOURCE
} Error;

Error tokenize(char* source, char** beg, char** end) {
	Error err = ERROR_NONE;
	if (!source || !beg || !end) {
		/* TODO: error handling */
		err = ERROR_SOURCE;
		printf("Error: No source file");
		return err;
	}

	*beg = source;
	*beg += strspn(*beg, whitespace); // addig számol ameddig nem talál, szóval ha az első már nem white space akkor 0-t ad vissza
	*end = *beg;
	*end += strcspn(*end, delimiter); // addig számol ameddig talál, szóval az első alkalomig
	
	return err;
}

void push(int num) {
	stack_pointer++;
	stack[stack_pointer] = num;
}

int pop() {
	if (stack_pointer > 0) {
		stack_pointer--;
		return stack[stack_pointer + 1];
	}
	else {
		printf("ERROR: the stack is empty!");
		exit(1);
		return 0;
	}
}

void add() {
	int a = pop();
	int b = pop();
	push(a + b);
}

void sub() {
	int a = pop();
	int b = pop();
	push(b - a);
}

#define TOKEN_PUSH 	"PUSH"
#define TOKEN_TOP 	"TOP"
#define TOKEN_ADD 	"ADD"
#define TOKEN_SUB 	"SUB"
#define TOKEN_PRINT "PRINT"
#define TOKEN_LB	"LB"
#define TOKEN_EXIT 	"EXIT"
#define TOKEN_LABEL "label"
#define TOKEN_RUN 	"RUN"
#define TOKEN_IF 	"IF"
#define TOKEN_INPUT	"INPUT"

typedef struct Label {
	char* name;
	int first_token;
	int last_token;
} Label;

Label labels[32];
int labels_counter = 0;

void add_label(int start, int end, char* name) {
	Label l = {
		.name = name,
		.first_token = start,
		.last_token = end
	};

	labels[labels_counter++] = l; 
}

void run(int start, int end) {
	for (size_t i = start; i < end; i++) {
		if (strcmp(tokens[i], TOKEN_PUSH) == 0) {
			push(atoi(tokens[i+1]));
		} else if (strcmp(tokens[i], TOKEN_TOP) == 0) {
			printf("%i\n", pop());
		} else if (strcmp(tokens[i], TOKEN_ADD) == 0) {
			add();
		} else if (strcmp(tokens[i], TOKEN_SUB) == 0) {
			sub();
		} else if (strcmp(tokens[i], TOKEN_PRINT) == 0) {
			for (size_t j = 1; j < strlen(tokens[i+1]) - 1; j++) {
				printf("%c", tokens[i+1][j]);
			}
			printf(" ");
			
		} else if (strcmp(tokens[i], TOKEN_LB) == 0) {
			printf("\n");
		}
		else if (strcmp(tokens[i], TOKEN_EXIT) == 0) {
			exit(0);
		} else if (strcmp(tokens[i], TOKEN_RUN) == 0) {
			for (size_t j = 0; j < labels_counter; j++) {
				char* name = labels[j].name;
				name[strlen(name)-1] = '\0';
				if (strcmp(name, tokens[i+1]) == 0) {
					run(labels[j].first_token, labels[j].last_token);
				}
			}
		} else if (strcmp(tokens[i], TOKEN_IF) == 0) {
			int top = stack[stack_pointer];
			if (top == 0) {
				for (size_t j = 0; j < labels_counter; j++) {
					char* name = labels[j].name;
					name[strlen(name)] = '\0';
					if (strcmp(name, tokens[i+1]) == 0) {
						run(labels[j].first_token, labels[j].last_token);
						break;
					}
				}
			}
		} else if (strcmp(tokens[i], TOKEN_INPUT) == 0) {
			int input;
			scanf("%d", &input);
			if (input) {
				push(input);			
			} else {
				printf("ERROR!\n");
				exit(0);
			}
		}
	}
}

void labelize() {
	for (size_t i = 0; i < token_counter; ++i) {
		if (strcmp(tokens[i], TOKEN_LABEL) == 0) {
			int end_index = i;
			while (strcmp(tokens[end_index], "end") != 0) {
				end_index++;
			}

			add_label(i+1, end_index, tokens[i+1]);
		}
	}
}

int main(int argc, char** argv) {

	if (argc < 2) {
		usage(); 
		exit(1); 
	} 
	else {
		char* content = read_file(argv[1]);
		
		char* beg = content;
		char* end = content;
		char* token = malloc(end - beg + 1);

		while (tokenize(end, &beg, &end) == ERROR_NONE) {
			if (end - beg == 0) break;
			memcpy(token, beg, end - beg);
			token[end-beg] = '\0';
			strcpy(tokens[token_counter++], token);
		}

		int main_index = 0;
		while(strcmp(tokens[main_index], "main:") != 0) {
			main_index++;	
		}
		int end_index = main_index;
		while (strcmp(tokens[end_index], "end") != 0) {
			end_index++;
		}

		labelize();
		run(main_index, end_index);
		free(content);
	}

	exit(0);
}