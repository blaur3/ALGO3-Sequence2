// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


/** 
 * Utilities function to print the token queues
 */
void print_token(const void* e, void* user_param);
void print_queue(FILE* f, Queue* q);

/** 
 * Function to be written by students
 */


 // (’+’, ’−’, ’∗’, ’/’, ’∧’, ’(’ ou ’)’)
bool isSymbol(char c){
	switch(c){
		case '+': return 1;
		case '-': return 1;
		case '*': return 1;
		case '/': return 1;
		case '^': return 1;
		case '(': return 1;
		case ')': return 1;
		default: return 0;
	}
}


 Queue* stringToTokenQueue(const char* expression){
	Queue* q = create_queue();
	const char* curpos = expression;
	Token * token;
	int count = 0;

	while(*curpos != '\0'){//lecture de expression
		if(isspace(*curpos)){	//case : blank, return
			curpos++;
		}
		else if(isSymbol(*curpos)){	//case : symbol
			token = create_token_from_string(curpos, 1);
			curpos++;
		}
		else{
			while(!isspace(*(curpos+count)) && !isSymbol(*(curpos+count)) && *(curpos+count) != '\0'){	//case num
				count +=1;
			}
			token = create_token_from_string(curpos, count);
			curpos = curpos + count;			
			count = 0;
		}

		queue_push(q,token);
	}
	return q;
}


void computeExpressions(FILE* input) {
	//(void)input;
	char *buffer;
    size_t bufsize = 512;
    size_t characters;

	//allocation du buffer
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

	//lecture de input
	while((characters = getline(&buffer,&bufsize,input)) != (size_t)-1){
		if(!isblank(buffer[0])){
			printf("Input  :   %s",buffer);
			Queue * q = stringToTokenQueue(buffer);
			print_queue(input, q);
		}

	}

	//erreur lecture
	if (characters != (size_t)-1) {
            perror("Erreur de lecture");
    }
    
	free(buffer);
}


/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */
int main(int argc, char** argv){
	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}
	
	FILE* input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}
	computeExpressions(input);

	fclose(input);
	return 0;
}
 
void print_token(const void* e, void* user_param) {
	FILE* f = (FILE*)user_param;
	Token* t = (Token*)e;
	token_dump(f, t);
}

void print_queue(FILE* f, Queue* q) {
	fprintf(f, "(%d) --  ", queue_size(q));
	queue_map(q, print_token, f);
}

