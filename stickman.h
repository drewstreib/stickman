#ifndef STICKMAN_H
#define STICKMAN_H

#include <stdio.h>
#include <stdlib.h>

/* Terminal dimensions */
#define TERMINAL_WIDTH  80
#define TERMINAL_HEIGHT 24

/* Animation settings */
#define ANIMATION_DELAY 100000 /* microseconds */
#define MAX_FRAMES      20
#define MAX_FILENAME_LENGTH 512
#define ANIMATION_DIR   "anim"

/* Frame structure */
typedef struct {
    char content[TERMINAL_HEIGHT][TERMINAL_WIDTH + 1];
} Frame;

#endif /* STICKMAN_H */
