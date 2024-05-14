/*
## Task 2 (Postări și reposturi)

Pentru această etapă, vom lucra cu o structură de date arborescentă, care va fi folosită pentru a stoca postările și reposturile. Fiecare postare va avea un id unic și va putea avea mai multe reposturi. Un repost va fi o postare în sine, care va avea un id unic și va fi legată de postarea originală. Vom folosi un arbore pentru a stoca aceste postări și reposturi, unde fiecare nod va avea un câmp `events`, care va fi un arbore de postări și reposturi.

```
struct {
    id: number,
    titlu: string,
    user_id: number,
    events: tree
}
```

### Creează o postare

`create <nume> <titlu>`

Utilizatorul `<nume>` va crea o postare cu titlul `<titlu>`. Fiecare postare va avea un id unic, care va fi incrementat la fiecare postare nouă.

Exemplu:

```
> create Mihai "Titlu postare"
< Created "Titlu postare" for Mihai
```
*/



/*

struct info {
	int id;
	char *title;
	int user_id;
};

struct node_tree {
	info *data;
	node_tree **children;
};
}

struct tree {
	node_tree *root;
};
}
struct {
	tree *events;
}

1. Am creat structurile necesare pentru a stoca postarile si reposturile si ma gandeam
sa folosesc un vector de arbori si la fiecare arbore[i] sa fie rootul, postarea i si sa
aiba arborele cu toate reposturile.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

void handle_input_posts(char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *name = strtok(NULL, " ");
    	char *title = strtok(NULL, "\"");
    	title = strtok(NULL, "\""); // pentru a obține titlul între ghilimele
    	create_post(name, title);
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
	} else if (!strcmp(cmd, "common-repost")) {
		char *post_id_str = strtok(NULL, " ");
    	int post_id = atoi(post_id_str);
    	char *repost_id_1_str = strtok(NULL, " ");
    	int repost_id_1 = atoi(repost_id_1_str);
    	char *repost_id_2_str = strtok(NULL, " ");
    	int repost_id_2 = atoi(repost_id_2_str);
		int common = common_repost(post_id, repost_id_1, repost_id_2);
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

