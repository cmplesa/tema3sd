
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"
#include <errno.h>

#define MAX_POSTS 100
#define MAX_PEOPLE 550

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

node_posts_t *create_post_node(char *name, char *title,
							   int post_id, int repost_activity, int parent_id)
{
	node_posts_t *new_post = (node_posts_t *)malloc(sizeof(node_posts_t));
	DIE(!new_post, "new_node malloc");
	new_post->likes = 0;
	new_post->user_id = get_user_id(name);
	new_post->parent_id = parent_id;
	new_post->post_id = post_id;
	new_post->children = malloc(MAX_POSTS * sizeof(node_posts_t *));
	new_post->children_number = 0;
	new_post->like_list = calloc(MAX_PEOPLE, sizeof(int));
	DIE(!new_post->like_list, "new_node->like_list malloc");
	DIE(!new_post->children, "new_node->children malloc");
	if (repost_activity == 0)
		new_post->title = strdup(title);
	else
		new_post->title = NULL;
	return new_post;
}

post_tree_t *create_post_tree(void)
{
	post_tree_t *new_tree = malloc(sizeof(post_tree_t));
	DIE(!new_tree, "new_tree malloc");
	new_tree->root = NULL;
	return new_tree;
}

post_array_t **create_post(post_array_t **tree_of_posts,
						   char *name, char *title)
{
	(*tree_of_posts)->number_of_posts++;
	(*tree_of_posts)->total_posts++;
	int num_posts = (*tree_of_posts)->number_of_posts;
	if (num_posts == 1) { // Init first tree
		(*tree_of_posts)->posts = malloc(1 * sizeof(post_tree_t *));
		DIE(!(*tree_of_posts)->posts, "tree_of_posts->posts malloc");
	} else { // Realloc tree array
		post_tree_t **bigger_tree = realloc((*tree_of_posts)->posts,
		(num_posts) * sizeof(post_tree_t *));
		DIE(!bigger_tree, "bigger_tree malloc");
		(*tree_of_posts)->posts = bigger_tree;
	}
	(*tree_of_posts)->posts[num_posts - 1] = create_post_tree();
	(*tree_of_posts)->posts[num_posts - 1]->root =
	create_post_node(name, title, (*tree_of_posts)->total_posts, 0, -1);
	printf("Created %s for %s\n",
		   (*tree_of_posts)->posts[num_posts - 1]->root->title, name);

	return tree_of_posts;
}

void repost_post(post_array_t **tree_of_posts, char *name, int post_id)
{
	int new_post_id = 0, root_id = 0, repost_nr = 0;
	for (int i = 0; i < (*tree_of_posts)->number_of_posts; i++) {
		if ((*tree_of_posts)->posts[i]->root->post_id == post_id) {
			root_id = i;
			repost_nr = (*tree_of_posts)->posts[i]->root->children_number;
			(*tree_of_posts)->posts[i]->root->children_number++;
			break;
		}
	}
	new_post_id = ++(*tree_of_posts)->total_posts;
	printf("Created repost #%d for %s\n", new_post_id, name);
	(*tree_of_posts)->posts[root_id]->root->children[repost_nr] =
	create_post_node(name, NULL, new_post_id, 1, post_id);
}

node_posts_t *find_node_by_id(node_posts_t *root, int post_id)
{
	if (root->post_id == post_id)
		return root;
	for (int i = 0; i < root->children_number; i++) {
		node_posts_t *found = find_node_by_id(root->children[i], post_id);
		if (found)
			return found;
	}
	return NULL;
}

void repost_repost(post_array_t **tree_of_posts,
				   char *name, int post_id, int repost_id)
{
	int new_post_id = 0;
	for (int i = 0; i < (*tree_of_posts)->number_of_posts; i++) {
		if ((*tree_of_posts)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*tree_of_posts)->posts[i]->root;
			node_posts_t *found = find_node_by_id(root, repost_id);

			new_post_id = ++(*tree_of_posts)->total_posts;
			found->children[found->children_number] =
			create_post_node(name, NULL, new_post_id, 1, found->post_id);
			found->children_number++;
			break;
		}
	}
	printf("Created repost #%d for %s\n", new_post_id, name);
}

void common_repost(post_array_t **post_array, int post_id,
				   int repost_id_1, int repost_id_2)
{
	int parent_id_1 = repost_id_1, parent_id_2 = repost_id_2;
	int orig_id_1 = repost_id_1, orig_id_2 = repost_id_2;
	int ok = 1;
	node_posts_t *root = NULL;
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			root = (*post_array)->posts[i]->root;
			break;
		}
	}
	node_posts_t *node_1 = find_node_by_id(root, repost_id_1);
	node_posts_t *node_2 = find_node_by_id(root, repost_id_2);
	while (parent_id_1 != parent_id_2) {
		if (repost_id_1 != root->post_id)
			node_1 = find_node_by_id(root, repost_id_1);

		if (repost_id_2 != root->post_id)
			node_2 = find_node_by_id(root, repost_id_2);

		if (!node_1) {
			printf("Repost 1 not found\n");
			return;
		} else if (!node_2) {
			printf("Repost 2 not found\n");
			return;
		}
		repost_id_1 = node_1->parent_id;
		repost_id_2 = node_2->parent_id;
		parent_id_1 = node_1->parent_id;
		parent_id_2 = node_2->parent_id;
		if (parent_id_1 == orig_id_2) {
			ok = 0;
			printf("The first common repost of %d and %d is %d\n",
				   orig_id_1, orig_id_2, orig_id_2);
			break;
		} else if (parent_id_2 == orig_id_1) {
			ok = 0;
			printf("The first common repost of %d and %d is %d\n",
				   orig_id_1, orig_id_2, orig_id_1);
			break;
		}
	}
	if (ok == 1)
		printf("The first common repost of %d and %d is %d\n",
			   orig_id_1, orig_id_2, parent_id_1);
}

void like_post(post_array_t **post_array, char *name, int post_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			if (root->like_list[get_user_id(name)] == 1) {
				printf("User %s unliked post %s\n", name, root->title);
				root->like_list[get_user_id(name)] = 0;
				root->likes--;
			} else {
				printf("User %s liked post %s\n", name, root->title);
				root->like_list[get_user_id(name)] = 1;
				root->likes++;
			}
			return;
		}
	}
}

void like_repost(post_array_t **post_array, char *name,
				 int post_id, int repost_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			node_posts_t *found = find_node_by_id(root, repost_id);
			if (found->like_list[get_user_id(name)] == 1) {
				printf("User %s unliked repost %s\n", name, root->title);
				found->like_list[get_user_id(name)] = 0;
				found->likes--;
			} else {
				printf("User %s liked repost %s\n", name, root->title);
				found->like_list[get_user_id(name)] = 1;
				found->likes++;
			}
			return;
		}
	}
}

void get_likes_post(post_array_t **post_array, int post_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			printf("Post %s has %d likes\n", root->title, root->likes);
			return;
		}
	}
}

void get_likes_repost(post_array_t **post_array, int post_id, int repost_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			node_posts_t *found = find_node_by_id(root, repost_id);
			printf("Repost #%d has %d likes\n", found->post_id, found->likes);
			return;
		}
	}
}

void ratio_post(post_array_t **post_array, int post_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			int max_likes = root->likes;
			int max_id = root->post_id;
			for (int j = 0; j < root->children_number; j++) {
				if (!root->children[j])
					continue;

				if (root->children[j]->likes > max_likes) {
					max_likes = root->children[j]->likes;
					max_id = root->children[j]->post_id;
				}
			}
			if (max_id == root->post_id)
				printf("The original post is the highest rated\n");
			else
				printf("Post %d got ratio'd by repost %d\n", post_id, max_id);
			return;
		}
	}
}

void delete_post(post_array_t **post_array, int post_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			printf("Deleted %s\n", (*post_array)->posts[i]->root->title);
			free_post_tree(&((*post_array)->posts[i]->root));
			free((*post_array)->posts[i]);
			(*post_array)->number_of_posts--;
			return;
		}
	}
}

void delete_repost(post_array_t **post_array, int post_id, int repost_id)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		if ((*post_array)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*post_array)->posts[i]->root;
			node_posts_t *found = find_node_by_id(root, repost_id);
			printf("Deleted repost #%d of post %s\n",
				   found->post_id, root->title);
			found->post_id = -1;
			return;
		}
	}
}

void print_reposts(node_posts_t *root)
{
	for (int i = 0; i < root->children_number; i++) {
		if (root->children[i]->post_id == -1)
			continue;

		printf("Repost #%d by %s\n", root->children[i]->post_id,
			   get_user_name(root->children[i]->user_id));
		print_reposts(root->children[i]);
	}
}

void get_reposts_post(post_array_t **tree_of_posts, int post_id)
{
	for (int i = 0; i < (*tree_of_posts)->number_of_posts; i++) {
		if ((*tree_of_posts)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*tree_of_posts)->posts[i]->root;
			printf("%s - Post by %s\n", root->title,
				   get_user_name(root->user_id));
			print_reposts(root);
			return;
		}
	}
}

void get_reposts_repost(post_array_t **tree_of_posts,
						int post_id, int repost_id)
{
	for (int i = 0; i < (*tree_of_posts)->number_of_posts; i++) {
		if ((*tree_of_posts)->posts[i]->root->post_id == post_id) {
			node_posts_t *root = (*tree_of_posts)->posts[i]->root;
			node_posts_t *found = find_node_by_id(root, repost_id);
			printf("Repost #%d by %s\n", found->post_id,
				   get_user_name(found->user_id));
			print_reposts(found);
			return;
		}
	}
}

void free_post_tree(node_posts_t **root)
{
	for (int i = 0; i < (*root)->children_number; i++) {
		if (!(*root)->children[i])
			continue;

		free_post_tree(&((*root)->children[i]));
	}
	free((*root)->like_list);
	free((*root)->children);
	if ((*root)->title)
		free((*root)->title);

	free(*root);
}

void free_post_array(post_array_t **post_array)
{
	for (int i = 0; i < (*post_array)->number_of_posts; i++) {
		free_post_tree(&((*post_array)->posts[i]->root));
		free((*post_array)->posts[i]);
	}
	if ((*post_array)->number_of_posts > 0)
		free((*post_array)->posts);
}

void handle_input_posts(char *input, post_array_t **post_array)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *name = strtok(NULL, " ");
		char *title = malloc(300 * sizeof(char));
		int x = strlen(name) + strlen(cmd) + 2;
		int beta = strlen(input) - 1;
		for (int i = x; i < beta; i++)
			title[i - x] = input[i];
		title[beta - x] = '\0';
		post_array = create_post(post_array, name, title);
		free(title);
	} else if (!strcmp(cmd, "repost")) {
		char *name = strtok(NULL, " ");
		char *post_id_string = strtok(NULL, "\n ");
		int post_id = atoi(post_id_string);
		char *repost_id_str = strtok(NULL, "\n ");
		if (!repost_id_str) {
			repost_post(post_array, name, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			repost_repost(post_array, name, post_id, repost_id);
		}
	} else if (!strcmp(cmd, "common-repost")) {
		char *aux = strtok(NULL, "\n");
		int post_id, repost_id_1, repost_id_2;
		if (sscanf(aux, "%d %d %d", &post_id, &repost_id_1, &repost_id_2) == 3)
			common_repost(post_array, post_id, repost_id_1, repost_id_2);
	} else if (!strcmp(cmd, "like")) {
		char *name = strtok(NULL, " ");
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if (!repost_id_str) {
			like_post(post_array, name, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			like_repost(post_array, name, post_id, repost_id);
		}
	} else if (!strcmp(cmd, "ratio")) {
		char *post_id_str = strtok(NULL, "\n ");
		int post_id = atoi(post_id_str);
		ratio_post(post_array, post_id);
	} else if (!strcmp(cmd, "delete")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, "\n ");
		if (!repost_id_str) {
			delete_post(post_array, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			delete_repost(post_array, post_id, repost_id);
		}
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if (!repost_id_str) {
			get_likes_post(post_array, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			get_likes_repost(post_array, post_id, repost_id);
		}
	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id_str = strtok(NULL, " ");
		int post_id = atoi(post_id_str);
		char *repost_id_str = strtok(NULL, " ");
		if (!repost_id_str) {
			get_reposts_post(post_array, post_id);
		} else {
			int repost_id = atoi(repost_id_str);
			get_reposts_repost(post_array, post_id, repost_id);
		}
	}
	free(commands);
}
