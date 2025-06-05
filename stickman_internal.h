#ifndef STICKMAN_INTERNAL_H
#define STICKMAN_INTERNAL_H

#include "stickman.h"

/* Internal functions exposed for testing */
/* Note: These are implementation details and should not be used by external code */

int compare_filenames(const void* a, const void* b);
int load_frame(const char* filename, Frame* frame);
int stickman_main(int argc, char* argv[]);

#endif /* STICKMAN_INTERNAL_H */
