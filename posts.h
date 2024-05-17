#ifndef POSTS_H
#define POSTS_H
#include <stdint.h>

typedef struct node_posts_t node_posts_t;
struct  node_posts_t {
	node_posts_t **children;
	int parent_id;
	int likes;
	uint16_t user_id;
	char *title;
	int post_id;
	int children_number;
	int *like_list;
};

typedef struct post_tree_t post_tree_t;
struct post_tree_t {
	node_posts_t  *root;
};

typedef struct post_array_t post_array_t;
struct post_array_t {
	post_tree_t **posts;
	int number_of_posts;
	int total_posts;
};

void free_post_tree(node_posts_t **root);

void free_post_array(post_array_t **post_array);

void handle_input_posts(char *input, post_array_t **posts);

#endif // POSTS_H
