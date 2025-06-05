#define _GNU_SOURCE

#include "stickman.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <unistd.h>

/* Global variables for signal handling */
static volatile bool g_running     = true;
static int           g_frame_count = 0;
static char**        g_filenames   = NULL;

/* Terminal control functions */
static void clear_screen(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

static void move_cursor(int row, int col)
{
    printf("\033[%d;%dH", row + 1, col + 1);
}

static void hide_cursor(void)
{
    printf("\033[?25l");
    fflush(stdout);
}

static void show_cursor(void)
{
    printf("\033[?25h");
    fflush(stdout);
}

static void setup_terminal(void)
{
    clear_screen();
    hide_cursor();
}

static void restore_terminal(void)
{
    show_cursor();
    clear_screen();
}

/* Signal handler for graceful exit */
static void handle_signal(int sig)
{
    (void)sig; /* Unused parameter */
    g_running = false;
}

/* Cleanup function */
static void cleanup(void)
{
    restore_terminal();

    if (g_filenames) {
        for (int i = 0; i < g_frame_count; i++) {
            free(g_filenames[i]);
        }
        g_filenames = NULL;
    }

    g_frame_count = 0;
}

/* Frame loading function */
static int load_frame(const char* filename, Frame* frame)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return 0;
    }

    memset(frame->content, ' ', sizeof(frame->content));

    char buffer[TERMINAL_WIDTH * 2]; /* Extra space for long lines */

    for (int i = 0; i < TERMINAL_HEIGHT; i++) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            /* Fill remaining lines with spaces */
            for (; i < TERMINAL_HEIGHT; i++) {
                memset(frame->content[i], ' ', TERMINAL_WIDTH);
                frame->content[i][TERMINAL_WIDTH] = '\0';
            }
            break;
        }

        /* Remove all types of line endings (LF, CR, CRLF) */
        size_t len = strlen(buffer);
        while (len > 0 &&
               (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[--len] = '\0';
        }

        /* Copy up to TERMINAL_WIDTH characters */
        size_t copy_len = len < TERMINAL_WIDTH ? len : TERMINAL_WIDTH;
        memcpy(frame->content[i], buffer, copy_len);

        /* Pad with spaces */
        for (size_t j = copy_len; j < TERMINAL_WIDTH; j++) {
            frame->content[i][j] = ' ';
        }
        frame->content[i][TERMINAL_WIDTH] = '\0';
    }

    fclose(file);
    return 1;
}

/* Frame display function */
static void display_frame(const Frame* current, const Frame* previous)
{
    for (int row = 0; row < TERMINAL_HEIGHT; row++) {
        for (int col = 0; col < TERMINAL_WIDTH; col++) {
            if (current->content[row][col] != previous->content[row][col]) {
                move_cursor(row, col);
                putchar(current->content[row][col]);
            }
        }
    }
    fflush(stdout);
}

/* Filename comparison for sorting */
static int compare_filenames(const void* a, const void* b)
{
    const char* const* str_a = (const char* const*)a;
    const char* const* str_b = (const char* const*)b;
    return strcmp(*str_a, *str_b);
}

/* Load all animation frames from directory */
static int load_animation_frames(Frame frames[], int* frame_count)
{
    DIR* dir = opendir(ANIMATION_DIR);
    if (!dir) {
        perror("opendir");
        return 0;
    }

    *frame_count = 0;
    const struct dirent* entry;

    while ((entry = readdir(dir)) != NULL && *frame_count < MAX_FRAMES) {
        if (entry->d_name[0] != '.') {
            g_filenames[*frame_count] = malloc(MAX_FILENAME_LENGTH);
            if (!g_filenames[*frame_count]) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                closedir(dir);
                return 0;
            }
            snprintf(g_filenames[*frame_count], MAX_FILENAME_LENGTH, "%s/%s",
                     ANIMATION_DIR, entry->d_name);
            (*frame_count)++;
        }
    }
    closedir(dir);

    if (*frame_count == 0) {
        fprintf(stderr, "Error: No animation frames found in %s directory\n",
                ANIMATION_DIR);
        return 0;
    }

    /* Sort filenames for consistent ordering */
    qsort(g_filenames, (size_t)*frame_count, sizeof(char*), compare_filenames);

    /* Load each frame */
    for (int i = 0; i < *frame_count; i++) {
        if (!load_frame(g_filenames[i], &frames[i])) {
            fprintf(stderr, "Error: Could not load frame %s\n", g_filenames[i]);
            return 0;
        }
    }

    return 1;
}

/* Main animation loop */
static void run_animation(Frame frames[], int frame_count)
{
    Frame empty_frame;
    memset(&empty_frame, ' ', sizeof(empty_frame));
    for (int i = 0; i < TERMINAL_HEIGHT; i++) {
        empty_frame.content[i][TERMINAL_WIDTH] = '\0';
    }

    Frame* previous_frame      = &empty_frame;
    int    current_frame_index = 0;

    while (g_running) {
        display_frame(&frames[current_frame_index], previous_frame);
        previous_frame      = &frames[current_frame_index];
        current_frame_index = (current_frame_index + 1) % frame_count;
        usleep(ANIMATION_DELAY);
    }
}

/* Usage information */
static void print_usage(const char* program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
    printf("  -d, --delay    Set animation delay in milliseconds (default: "
           "100)\n");
    printf("\nPress Ctrl+C to stop the animation.\n");
}

/* Main function */
int main(int argc, char* argv[])
{
    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("stickman version 1.0.0\n");
            return 0;
        }
    }

    /* Allocate memory for frames and filenames */
    Frame frames[MAX_FRAMES];
    g_filenames = calloc(MAX_FRAMES, sizeof(char*));
    if (!g_filenames) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    /* Set up signal handlers */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Load animation frames */
    if (!load_animation_frames(frames, &g_frame_count)) {
        cleanup();
        return EXIT_FAILURE;
    }

    /* Set up terminal and run animation */
    setup_terminal();
    run_animation(frames, g_frame_count);

    /* Clean up and exit */
    cleanup();
    return EXIT_SUCCESS;
}
