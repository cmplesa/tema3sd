#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "posts.h"

int check_if_friend(int ***relations, int user_id, int friend_id)
{
	if (user_id == friend_id || (*relations)[user_id][friend_id] == 1) {
		return 1;
	}
	return 0;
}

void feed(post_array_t **post_array, int ***relations, char *name, int feed_size)
{
	for (int i = (*post_array)->number_of_posts - 1; i >= 0; i--) {
		node_posts_t *root = (*post_array)->posts[i]->root;
		if (check_if_friend(relations, get_user_id(name), root->user_id)) {
				printf("%s: %s\n", get_user_name(root->user_id), root->title);
				feed_size--;
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
		}
	}
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if((*post_array)->posts[i]->root->user_id != get_user_id(name)) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			print_node_by_user_id_repost(root, get_user_id(name), root->title);
		}
	}
}

void print_friends_that_reposted(int ***relations, node_posts_t *root, int user_id, int post_id)
{
	if (check_if_friend(relations, root->user_id, user_id) && get_user_name(root->user_id) != get_user_name(user_id)){
		printf("%s\n", get_user_name(root->user_id));
	}
	for (int i = 0; i < root->children_number; i++) {
		print_friends_that_reposted(relations, root->children[i], user_id, post_id);
	}
}

void see_friends_reposted(int ***relations , post_array_t **post_array, char *name, int post_id)
{	
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			print_friends_that_reposted(relations, root, get_user_id(name), post_id);
			break;
		}
	}	
}

void see_common_group(int ***relations, char *name)
{
	int user_id = get_user_id(name);
	int *friends = malloc(MAX_PEOPLE * sizeof(int));
	int friends_size = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if ((*relations)[user_id][i] == 1 || i == user_id) {
			friends[friends_size] = i;
			friends_size++;
		}
	}
	while (1) {
		int min_common_friends = friends_size;
		int min_common_friend = -1;
		for (int i = 0; i < friends_size; i++) {
			int common_friends = 0;
			for (int j = 0; j < friends_size; j++) {
				if ((*relations)[friends[i]][friends[j]] == 1) {
					common_friends++;
				}
			}
			if (common_friends < min_common_friends) {
				min_common_friends = common_friends;
				min_common_friend = i;
			}
		}
		if (min_common_friends < friends_size - 1) {
			for (int i = min_common_friend; i < friends_size - 1; i++) {
				friends[i] = friends[i + 1];
			}
			friends_size--;
		} else {
			break;
		}
	}
	printf("The closest friend group of %s is:\n", name);
	for (int i = 0; i < friends_size; i++) {
		printf("%s\n", get_user_name(friends[i]));
	}
	free(friends);
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
		see_friends_reposted(relations, post_array, name, post_id);
	} else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, " \n");
		see_common_group(relations, name);
	}
	free(commands);
}
