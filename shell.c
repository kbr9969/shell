#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_PATH_LENGTH 1024


// Splits the string by space and returns the array of tokens
char **tokenize(char *line) {

    // tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
	for(i=0; i < strlen(line); i++){

		char readChar = line[i];

        // tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
				tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else {
			token[tokenIndex++] = readChar;
		}
	}
	
	free(token);
	tokens[tokenNo] = NULL ;
	return tokens;
}

void task(char **tokens, bool sequential)
{
	if(strcmp(tokens[0], "cd") == 0)
	{	//Fork not needed as one intents to change the cwd of the parent process
		if(tokens[1] == NULL || (tokens[1] != NULL && tokens[2] != NULL))
		{
			printf("cd: Incorrect number of arguments :/\n");
		}
		else
		{
			const char* path = tokens[1];
			chdir(path);
		}
	}
	else if(strcmp(tokens[0], "exit") == 0)
	{
		exit(0);
	}
	else
	{
		int x  = fork();
		if(x < 0)
		{
			printf("fork() failed with error: %d", (int) errno);
		}
		else if(x == 0)
		{
			execvp(tokens[0], tokens);
			exit(0);
		}
		else
		{
			if(sequential) wait(NULL);
		}
	}
}

int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;           

	while(1)
	{	
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		line[strlen(line)] = '\n'; //terminate with new line
		/* END: TAKING INPUT */

		char seperator[5] = "&&";
		bool sequential = true;
		if(strstr(line, "&&&"))
		{
			strcpy(seperator, "&&&");
			sequential = false;
		}
		char *linedivided = strtok(line, seperator); // seperate the line into atomic tasks
		while(linedivided != NULL)
		{
			tokens = tokenize(linedivided); // tokenize the command and it's arguments
			task(tokens, sequential);
			for(int i=0;tokens[i]!=NULL;i++) free(tokens[i]);
			free(tokens);
			linedivided = strtok(NULL, seperator);
		}
		while(wait(NULL) != -1); // &&& -> parallel execution and reaping all of them
	}
	return 0;
}


