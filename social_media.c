#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

int main(void)
{
	init_users();

	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);

	// Init relations matrix
	int **relations = malloc(MAX_PEOPLE * sizeof(int *));
	for (int i = 0; i < MAX_PEOPLE; i++) {
		relations[i] = malloc(MAX_PEOPLE * sizeof(int));
		for (int j = 0; j < MAX_PEOPLE; j++)
			relations[i][j] = 0;
	}

	// Init posts array
	post_array_t *posts = malloc(sizeof(post_array_t));
	posts->number_of_posts = 0;
	posts->total_posts = 0;

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, &relations);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &posts);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, &posts, &relations);
		#endif
	}

	// Free relations matrix
	for (int i = 0; i < MAX_PEOPLE; i++)
		free(relations[i]);
	free(relations);

	// Free posts array
	#ifdef TASK_2
	free_post_array(&posts);
	#endif
	free(posts);

	free_users();
	free(input);

	return 0;
}
