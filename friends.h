#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void add_friend(int ***relations, char *user1, char *user2);

void remove_friend(int ***relations, char *user1, char *user2);

void sort_names(char ***names, int names_number);

void print_suggestions(int **relations, char *user);

void print_distance(int **relations, char *user1, char *user2);

void print_common_friends(int **relations, char *user1, char *user2);

void print_friends(int **relations, char *user);

void print_popular(int **relations, char *user);

void handle_input_friends(char *input, int ***relations);

#endif // FRIENDS_H
