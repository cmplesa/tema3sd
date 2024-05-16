
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"
#include <errno.h>

#define MAX_POSTS 100

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

node_posts *create_post_node(char *name, char *title, int post_id, int repost_activity, int parent_id)
{	
	if (repost_activity == 0) {
		node_posts *new_post = (node_posts *)malloc(sizeof(node_posts));
		DIE(new_post == NULL, "new_node malloc");
		new_post->likes = 0;
		new_post->user_id = get_user_id(name);
		new_post->title = malloc(sizeof(strlen(title) + 1));
		printf("%s -> %ld\n", title, strlen(title) + 1);
		memcpy(new_post->title, title, strlen(title) + 1);
		new_post->parent_id = parent_id;
		new_post->post_id = post_id;
		new_post->children = malloc(MAX_POSTS * sizeof(node_posts *));
		DIE(new_post->children == NULL, "new_node->children malloc");
		for(int i = 0; i < MAX_POSTS; i++) {
			new_post->children[i] = malloc(sizeof(node_posts));
		}
		return new_post;
	} else {
		node_posts *new_repost = (node_posts *)malloc(sizeof(node_posts));
		DIE(new_repost == NULL, "new_node malloc");
		new_repost->likes = 0;
		new_repost->user_id = get_user_id(name);
		new_repost->title = NULL;
		new_repost->post_id = post_id;
		new_repost->parent_id = parent_id;
		new_repost->children = malloc(MAX_POSTS * sizeof(node_posts *));
		DIE(new_repost->children == NULL, "new_repost->children malloc");
		for(int i = 0; i < MAX_POSTS; i++) {
			new_repost->children[i] = malloc(sizeof(node_posts));
		}
		return new_repost;
	}
}

post_tree *create_post_tree(size_t data_size)
{
	post_tree *new_tree = malloc(sizeof(post_tree));
	new_tree->root = NULL;
	new_tree->data_size = data_size;
	return new_tree;
}

post_array **create_post(post_array **tree_of_posts, char *name, char *title, int *num_posts)
{	
	(*num_posts)++;
	//printf("%d\n", (*num_posts));
	(*tree_of_posts)->number_of_posts = (*num_posts);
	//printf("%d\n", (*tree_of_posts)->number_of_posts);
	(*tree_of_posts)->number_of_posts = (*num_posts);
	int number_of_posts = (*tree_of_posts)->number_of_posts;
	post_tree **bigger_tree = realloc((*tree_of_posts)->posts,(number_of_posts) * sizeof(post_tree *));
	DIE(bigger_tree == NULL, "bigger_tree malloc");
	(*tree_of_posts)->posts = bigger_tree;
	printf("Created %s for %d\n", title, get_user_id(name));
	//printf("%d", (*num_posts));
	(*tree_of_posts)->posts[(*num_posts)  - 1] = create_post_tree(sizeof(node_posts));
	
	(*tree_of_posts)->posts[(*num_posts) - 1]->root = create_post_node(name, title, number_of_posts, 0, -1);
	// printf("< Created %s for %d\n", (*tree_of_posts)->posts[0]->root->title, (*tree_of_posts)->posts[0]->root->user_id);

	return tree_of_posts;	
}

void repost_post(char *name, int post_id)
{
	printf("< %s reposted post %d\n", name, post_id);
}

void repost_repost(char *name, int post_id, int repost_id)
{
	printf("< %s reposted repost %d of post %d\n", name, repost_id, post_id);
}

void common_repost(int post_id, int repost_id_1, int repost_id_2)
{
	printf("< Common reposts between %d and %d: %d\n", repost_id_1, repost_id_2, post_id);
}

void like_post(char *name, int post_id)
{
	printf("< %s liked post %d\n", name, post_id);
}

void like_repost(char *name, int post_id, int repost_id)
{
	printf("< %s liked repost %d of post %d\n", name, repost_id, post_id);
}

void ratio_post(int post_id)
{
	printf("< Ratio for post %d: 0.5\n", post_id);
}

void delete_post(int post_id)
{
	printf("< Deleted post %d\n", post_id);
}

void delete_repost(int post_id, int repost_id)
{
	printf("< Deleted repost %d of post %d\n", repost_id, post_id);
}

void get_likes_repost(int post_id, int repost_id)
{
	printf("< Likes for repost %d of post %d: 1\n", repost_id, post_id);
}

void get_reposts_post(int post_id)
{
	printf("< Reposts for post %d: 1\n", post_id);
}

void get_reposts_repost(int post_id, int repost_id)
{
	printf("< Reposts for repost %d of post %d: 1\n", repost_id, post_id);
}

void get_likes_post(int post_id)
{
	printf("< Likes for post %d: 1\n", post_id);
}

void post(post_array **post_array)
{
	printf("< Posts: %d1\n", (*post_array)->number_of_posts);
}

void handle_input_posts(char *input, post_array **post_array)
{
    char *commands = strdup(input);
    char *cmd = strtok(commands, "\n ");
	int number_of_posts = 0;
    if (!cmd)
        return;

    if (!strcmp(cmd, "create")) {
		char *name = strtok(NULL, " ");
		char *title = malloc(300 * sizeof(char));
		int x = strlen(name) + strlen(cmd) + 2;
		int beta = strlen(input) - 1;
		for(int i = x; i < beta; i++) {
			title[i - x] = input[i];
		}
		title[beta - x] = '\0';
    	post_array = create_post(post_array, name, title, &number_of_posts);
	} else if (!strcmp(cmd, "repost")) {
		char *name = strtok(NULL, " ");
		char *post_id_string = strtok(NULL, "\" ");
		int post_id = atoi(post_id_string);
		char *repost_id_str = strtok(NULL, " \"");
		if(repost_id_str == NULL) {
			repost_post(name, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			repost_repost(name, post_id, repost_id);
		}
		post(post_array);
	} else if (!strcmp(cmd, "common-repost")) {
		char *post_id_str = strtok(NULL, " ");
    	int post_id = atoi(post_id_str);
    	char *repost_id_1_str = strtok(NULL, " ");
    	int repost_id_1 = atoi(repost_id_1_str);
    	char *repost_id_2_str = strtok(NULL, " ");
    	int repost_id_2 = atoi(repost_id_2_str);
		common_repost(post_id, repost_id_1, repost_id_2);
	} else if (!strcmp(cmd, "like")) {
		char *name = strtok(NULL, " ");
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if(repost_id_str == NULL) {
			like_post(name, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			like_repost(name, post_id, repost_id);
		}
	} else if (!strcmp(cmd, "ratio")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		ratio_post(post_id);
	} else if (!strcmp(cmd, "delete")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if(repost_id_str == NULL) {
			delete_post(post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			delete_repost(post_id, repost_id);
		}
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if(repost_id_str == NULL) {
			get_likes_post(post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			get_likes_repost(post_id, repost_id);
		}
	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if(repost_id_str == NULL) {
			get_reposts_post(post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			get_reposts_repost(post_id, repost_id);
		}
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if(repost_id_str == NULL) {
			get_likes_post(post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			get_likes_repost(post_id, repost_id);
		}
	}
	
}
