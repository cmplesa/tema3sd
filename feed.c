#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "posts.h"

/*feed <nume> <feed-size>

Afișează cele mai recente <feed-size> postări ale unui utilizator și ale prietenilor lui.

Exemplu:

> create Andrei "Prima mea postare" 
> create Mihai "Al doilea" 
> create Mihnea "Vand Golf 4" 
> create Alex "Buna TPU, merita sa dau la Poli?" 
> create Ana "Ati auzit ultima melodie a lui Kanye?" 
> create Luca "Nu-mi vine sa cred cine a castigat Grand Prix-ul de la Miami" 
> feed Andrei 5 
< Luca: "Nu-mi vine sa cred cine a castigat Grand Prix-ul de la Miami" 
< Ana: "Ati auzit ultima melodie a lui Kanye?" 
< Alex: "Buna TPU, merita sa dau la Poli?" 
< Mihnea: "Vand Golf 4" 
< Mihai: "Al doilea"*/

int check_if_friend(int ***relations, int user_id, int friend_id)
{
	if (user_id == friend_id) {
		return 1;
	}
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if ((*relations)[user_id][i] == 1) {
			return 1;
		}
	}
	return 0;
}

void print_reposts_feed(int ***relations, node_posts_t *root, int user_id, int *feed_size)
{
	for (int i = 0; i < root->children_number; i++) {
		if(root->children[i]->post_id == -1) {
			continue;
		}
		if ((*feed_size) == 0) {
			return;
		}
		print_reposts_feed(relations, root->children[i], user_id, feed_size);
		if (check_if_friend(relations, root->children[i]->user_id, user_id)) {
			printf("%s: %s\n", get_user_name(root->children[i]->user_id), root->children[i]->title);
			(*feed_size)--;
		}
	}
}

void feed(post_array_t **post_array, int ***relations, char *name, int feed_size)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->user_id == get_user_id(name)) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			printf("%s: %s\n", get_user_name(root->user_id), root->title);
			feed_size--;
			print_reposts_feed(relations, root, root->user_id, &feed_size);
		} else {
			node_posts_t *root = (*post_array)->posts[i]->root;
			if (check_if_friend(relations, get_user_id(name), root->user_id)) {
				printf("%s: %s\n", get_user_name(root->user_id), root->title);
				feed_size--;
			}
			print_reposts_feed(relations, root, get_user_id(name), &feed_size);
		}
	}
}

node_posts_t *print_node_by_user_id(node_posts_t *root, node_posts_t *root_check, int user_id)
{
	if (root->user_id == user_id && root != root_check) {
		printf("Reposted %s\n", root_check->title);
	}
	for (int i = 0; i < root->children_number; i++) {
		print_node_by_user_id(root->children[i], root_check, user_id);
	}
	return NULL;
}

node_posts_t *print_node_by_user_id_repost(node_posts_t *root, int user_id, char *title)
{
	if (root->user_id == user_id) {
		printf("Reposted: %s\n", title);
	}
	for (int i = 0; i < root->children_number; i++) {
		print_node_by_user_id_repost(root->children[i], user_id, title);
	}
	return NULL;
}

void view_profile(post_array_t **post_array,char *name)
{

	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if((*post_array)->posts[i]->root->user_id == get_user_id(name)) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			printf("Posted: %s\n", root->title);
			print_node_by_user_id(root, root, get_user_id(name));
		} else {
			node_posts_t *root = (*post_array)->posts[i]->root;
			print_node_by_user_id_repost(root, get_user_id(name), root->title);
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

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, " \n");
		char *feed_size_str = strtok(NULL, " \n");
		int feed_size = atoi(feed_size_str);
		feed(post_array, relations, name, feed_size);
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
