#ifndef POSTS_H
#define POSTS_H

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/

typedef struct node_posts node_posts;
struct  node_posts {
	/* left child */
	node_posts **children;
    int parent_id;
	/* data contained by the node */
	int likes;
    int user_id;
    char *title;
    int post_id;
};

typedef struct post_tree post_tree;
struct post_tree {
	/* root of the tree */
	node_posts  *root;
	int number_of_reposts;
	 /* size of the data contained by the nodes */
	size_t data_size;
};

typedef struct post_array post_array;
struct post_array {
	post_tree **posts;
	int number_of_posts;
};


void handle_input_posts(char *input, post_array **posts);

#endif // POSTS_H
