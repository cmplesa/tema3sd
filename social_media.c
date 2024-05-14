/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	int **relations = malloc(MAX_PEOPLE * sizeof(int *));
	for (int i = 0; i < MAX_PEOPLE; i++) {
		relations[i] = malloc(MAX_PEOPLE * sizeof(int));
		for (int j = 0; j < MAX_PEOPLE; j++) {
			relations[i][j] = 0;
		}
	}
	while (1) {
		input = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!input)
			break;

		#ifdef TASK_1
		handle_input_friends(input, &relations);
		#endif

		#ifdef TASK_2
		handle_input_posts(input);
		#endif

		#ifdef TASK_3
		handle_input_feed(input);
		#endif
	}
	for (int i = 0; i < MAX_PEOPLE; i++) {
		free(relations[i]);
	}
	free(relations);
	free(input);
	return 0;
}
