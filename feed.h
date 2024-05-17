#ifndef FEED_H
#define FEED_H

#include "posts.h"
#include "friends.h"

void handle_input_feed(char *input, post_array_t **posts, int ***relations);

#endif // FEED_H
