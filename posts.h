#ifndef POSTS_H
#define POSTS_H

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/

typedef struct node_posts_t node_posts_t;
struct  node_posts_t {
	
	node_posts_t **children;
	int parent_id;
	/* data contained by the node */
	int likes;
	uint16_t user_id;
	char *title;
	int post_id;
	int children_number;
	int *like_list;
};

typedef struct post_tree_t post_tree_t;
struct post_tree_t {
	/* root of the tree */
	node_posts_t  *root;
	int number_of_reposts;
	 /* size of the data contained by the nodes */
	size_t data_size;
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
