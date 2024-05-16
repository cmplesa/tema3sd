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
	post_array *posts = malloc(sizeof(post_array));
	posts->number_of_posts = 0;
	posts->posts = malloc(5 * sizeof(post_tree *));
	for(int i = 0; i < 5; i++) {
		posts->posts[i] = malloc(sizeof(post_tree));
		posts->posts[i]->root = NULL;
		posts->posts[i]->number_of_reposts = 0;
	}
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, &relations);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &posts);
		#endif

		#ifdef TASK_3
		handle_input_feed(input);
		#endif
	}
	
	for (int i = 0; i < MAX_PEOPLE; i++) {
		free(relations[i]);
	}
	free(relations);

	for(int i = 0; i < posts->number_of_posts; i++) {
		free(posts->posts[i]);
	}
	free(posts->posts);
	free(posts);

	free_users();
	free(input);

	return 0;
}
