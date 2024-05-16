#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "posts.h"

void feed(int ***relations,char *name, int feed_size)
{
	printf("Feed for %s\n", name);
	printf("Feed size: %d\n", feed_size);
	printf("%d", (*relations)[0][0]);
}


node_posts_t *print_node_by_user_id(node_posts_t *root, int user_id)
{
	if (root->user_id == user_id) {
		printf("Reposted %s", root->title);
	}
	for (int i = 0; i < root->children_number; i++) {
		print_node_by_user_id(root->children[i], user_id);
	}
	return NULL;
}

void view_profile(post_array_t **post_array,char *name)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if((*post_array)->posts[i]->root->user_id == get_user_id(name)) {
			printf("Posted %s", (*post_array)->posts[i]->root->title);
			print_node_by_user_id((*post_array)->posts[i]->root, get_user_id(name));
		}
	}
}

void see_friends_reposted(char *name, int post_id)
{
	printf("Friends that reposted %s\n", name);
	printf("Post id: %d\n", post_id);
}

void see_common_group(char *name)
{
	printf("Common groups for %s\n", name);
}

void handle_input_feed(char *input, post_array_t **post_array, int ***relations)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	handle_input_friends(input, relations);
	handle_input_posts(input, post_array);
	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, " \n");
		char *feed_size_str = strtok(NULL, " \n");
		int feed_size = atoi(feed_size_str);
		feed(relations,name, feed_size);
	} else if (!strcmp(cmd, "view-profile")) {
		char *name = strtok(NULL, " \n");
		view_profile(post_array, name);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *name = strtok(NULL, " \n");
		char *post_id_str = strtok(NULL, " \n");
		int post_id = atoi(post_id_str);
		see_friends_reposted(name, post_id);
	} else if (!strcmp(cmd, "common-groups")) {
		char *name = strtok(NULL, " \n");
		see_common_group(name);
	}
}

