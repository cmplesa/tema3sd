#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

void add_friend(int ***relations, char *user1, char *user2)
{
	int id1 = (int)get_user_id(user1);
	int id2 = (int)get_user_id(user2);
	(*relations)[id1][id2] = 1;
	(*relations)[id2][id1] = 1;
	printf("Added connection %s - %s\n", user1, user2);
}

void remove_friend(int ***relations, char *user1, char *user2)
{
	int id1 = (int)get_user_id(user1);
	int id2 = (int)get_user_id(user2);
	(*relations)[id1][id2] = 0;
	(*relations)[id2][id1] = 0;
	printf("Removed connection %s - %s\n", user1, user2);
}

void sort_names(char ***names, int names_number)
{
	for (int i = 0; i < names_number - 1; i++) {
		for (int j = i + 1; j < names_number; j++) {
			if (get_user_id((*names)[i]) > get_user_id((*names)[j])) {
				char *temp = (*names)[i];
				(*names)[i] = (*names)[j];
				(*names)[j] = temp;
			}
		}
	}
}

void uniqe_names(char ***names, int *names_number)
{
	for (int i = 0; i < (*names_number) - 1; i++) {
		// Daca se repeta se shifteaza toti o pozitie la stanga
		while (strcmp((*names)[i], (*names)[i + 1]) == 0 &&
			   i != (*names_number - 1)) {
			for (int j = i; j < (*names_number) - 1; j++)
				(*names)[j] = (*names)[j + 1];
			(*names_number)--;
		}
	}
}

void print_suggestions(int **relations, char *user)
{
	int id = (int)get_user_id(user);
	char **suggestions = malloc(MAX_PEOPLE * sizeof(char *));
	int suggestions_number = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) { // Salvez toate sugestiile
		if (relations[id][i] == 1) {
			for (int j = 0; j < MAX_PEOPLE; j++) {
				if (relations[i][j] == 1 && relations[id][j] == 0 && j != id) {
					suggestions[suggestions_number] = get_user_name(j);
					suggestions_number++;
				}
			}
		}
	}
	if (suggestions_number) {
		sort_names(&suggestions, suggestions_number);
		uniqe_names(&suggestions, &suggestions_number);
		printf("Suggestions for %s:\n", user);
		for (int i = 0; i < suggestions_number; i++)
			printf("%s\n", suggestions[i]);
	} else {
		printf("There are no suggestions for %s\n", user);
	}
	free(suggestions);
}

void print_distance(int **relations, char *user1, char *user2) // BFS
{
	int id1 = (int)get_user_id(user1);
	int id2 = (int)get_user_id(user2);
	if (id1 == id2) {
		printf("0\n");
		return;
	}
	int *visited = calloc(MAX_PEOPLE, sizeof(int));
	int *queue = malloc(MAX_PEOPLE * sizeof(int));
	int *distance = calloc(MAX_PEOPLE, sizeof(int));
	int print_idx = 0;
	int write_idx = 0;
	queue[write_idx] = id1; // Enqueue id1
	visited[id1] = 1;
	while (print_idx <= write_idx) {
		int curr = queue[print_idx++]; // Dequeue
		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (relations[curr][i] == 1 && visited[i] == 0) {
				queue[++write_idx] = i; // Enqueue i
				visited[i] = 1;
				distance[i] = distance[curr] + 1;
				if (i == id2) {
					printf("The distance between %s - %s is %d\n",
						   user1, user2, distance[i]);
					free(visited);
					free(queue);
					free(distance);
					return;
				}
			}
		}
	}
	printf("There is no way to get from %s to %s\n", user1, user2);
	free(visited);
	free(queue);
	free(distance);
}

void print_common_friends(int **relations, char *user1, char *user2)
{
	int id1 = (int)get_user_id(user1);
	int id2 = (int)get_user_id(user2);
	char **common_friends = malloc(MAX_PEOPLE * sizeof(char *));
	int common_number = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) { // Salvez toti prietenii comuni
		if (relations[id1][i] == 1 && relations[id2][i] == 1) {
			common_friends[common_number] = get_user_name(i);
			common_number++;
		}
	}
	sort_names(&common_friends, common_number);
	uniqe_names(&common_friends, &common_number);
	if (common_number) {
		printf("The common friends between %s and %s are:\n", user1, user2);
		for (int i = 0; i < common_number; i++)
			printf("%s\n", common_friends[i]);
	} else {
		printf("No common friends for %s and %s\n", user1, user2);
	}
	free(common_friends);
}

void print_friends(int **relations, char *user)
{
	int id = (int)get_user_id(user);
	int friends_number = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (relations[id][i] == 1)
			friends_number++;
	}
	printf("%s has %d friends\n", user, friends_number);
}

void print_popular(int **relations, char *user)
{
	int id = (int)get_user_id(user);
	int max_friends = 0;
	int popular_id = 0;
	int user_friends_nr = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (relations[id][i] == 1) {
			user_friends_nr++;
			int friends_number = 0;
			for (int j = 0; j < MAX_PEOPLE; j++)
				if (relations[i][j] == 1)
					friends_number++;
			if (friends_number > max_friends) { // Caut cei mai multi prieteni
				max_friends = friends_number;
				popular_id = i;
			}
		}
	}
	if (user_friends_nr >= max_friends) {
		printf("%s is the most popular\n", user);
	} else {
		printf("%s is the most popular friend of %s\n",
			   get_user_name(popular_id), user);
	}
}

void handle_input_friends(char *input, int ***relations)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		add_friend(relations, user1, user2);
	} else if (!strcmp(cmd, "remove")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		remove_friend(&(*relations), user1, user2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *user = strtok(NULL, "\n ");
		print_suggestions((*relations), user);
	} else if (!strcmp(cmd, "distance")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		print_distance((*relations), user1, user2);
	} else if (!strcmp(cmd, "common")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		print_common_friends((*relations), user1, user2);
	} else if (!strcmp(cmd, "friends")) {
		char *user = strtok(NULL, "\n ");
		print_friends((*relations), user);
	} else if (!strcmp(cmd, "popular")) {
		char *user = strtok(NULL, "\n ");
		print_popular((*relations), user);
	}

	free(commands);
}
