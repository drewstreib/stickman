#define _GNU_SOURCE
#ifndef TESTING
#define TESTING
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../stickman.h"
#include "../stickman_internal.h"

/* Simple test framework */
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    void test_##name(void); \
    void test_##name(void)

#define RUN_TEST(name) \
    do { \
        printf("Running test: %s... ", #name); \
        fflush(stdout); \
        tests_run++; \
        test_##name(); \
        tests_passed++; \
        printf("PASSED\n"); \
    } while (0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAILED\n  Expected: %d, Got: %d\n", (int)(expected), (int)(actual)); \
            exit(1); \
        } \
    } while (0)

#define ASSERT_STR_EQ(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf("FAILED\n  Expected: \"%s\", Got: \"%s\"\n", (expected), (actual)); \
            exit(1); \
        } \
    } while (0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf("FAILED\n  Condition was false: %s\n", #condition); \
            exit(1); \
        } \
    } while (0)

/* Functions to test are declared in stickman_internal.h */

/* Test data setup */
static void setup_test_files(void) {
    mkdir("test_anim", 0755);
    
    /* Create a simple test frame */
    FILE* f = fopen("test_anim/test_frame.txt", "w");
    if (f) {
        fprintf(f, "  o  \n");
        fprintf(f, " /|\\ \n");
        fprintf(f, "  |  \n");
        fprintf(f, " / \\ \n");
        fclose(f);
    }
    
    /* Create an empty test frame */
    f = fopen("test_anim/empty_frame.txt", "w");
    if (f) {
        fclose(f);
    }
}

static void cleanup_test_files(void) {
    unlink("test_anim/test_frame.txt");
    unlink("test_anim/empty_frame.txt");
    rmdir("test_anim");
}

/* Test compare_filenames function */
TEST(compare_filenames_basic) {
    const char* file1 = "frame01.txt";
    const char* file2 = "frame02.txt";
    const char* file3 = "frame01.txt";
    
    const char** ptr1 = &file1;
    const char** ptr2 = &file2;
    const char** ptr3 = &file3;
    
    /* file1 should come before file2 */
    ASSERT_TRUE(compare_filenames(ptr1, ptr2) < 0);
    
    /* file2 should come after file1 */
    ASSERT_TRUE(compare_filenames(ptr2, ptr1) > 0);
    
    /* file1 should equal file3 */
    ASSERT_EQ(0, compare_filenames(ptr1, ptr3));
}

TEST(compare_filenames_sorting) {
    /* Use malloc'd strings that can be reordered */
    char* files[3];
    files[0] = malloc(strlen("frame03.txt") + 1);
    files[1] = malloc(strlen("frame01.txt") + 1);
    files[2] = malloc(strlen("frame02.txt") + 1);
    strcpy(files[0], "frame03.txt");
    strcpy(files[1], "frame01.txt");
    strcpy(files[2], "frame02.txt");
    
    /* Sort the array of string pointers */
    qsort(files, 3, sizeof(char*), compare_filenames);
    
    /* Check they're in the right order */
    ASSERT_STR_EQ("frame01.txt", files[0]);
    ASSERT_STR_EQ("frame02.txt", files[1]);
    ASSERT_STR_EQ("frame03.txt", files[2]);
    
    /* Cleanup */
    free(files[0]);
    free(files[1]);
    free(files[2]);
}

/* Test load_frame function */
TEST(load_frame_basic) {
    setup_test_files();
    
    Frame frame;
    int result = load_frame("test_anim/test_frame.txt", &frame);
    
    ASSERT_EQ(1, result);  /* Should succeed */
    
    /* Check the first few characters of each line (ignoring padding) */
    ASSERT_TRUE(strncmp("  o  ", frame.content[0], 5) == 0);
    ASSERT_TRUE(strncmp(" /|\\ ", frame.content[1], 5) == 0);
    ASSERT_TRUE(strncmp("  |  ", frame.content[2], 5) == 0);
    ASSERT_TRUE(strncmp(" / \\ ", frame.content[3], 5) == 0);
    
    /* Check that lines beyond the file content are filled with spaces */
    for (int i = 4; i < TERMINAL_HEIGHT; i++) {
        ASSERT_EQ(' ', frame.content[i][0]);
        ASSERT_EQ('\0', frame.content[i][TERMINAL_WIDTH]);
    }
    
    cleanup_test_files();
}

TEST(load_frame_nonexistent) {
    Frame frame;
    int result = load_frame("nonexistent_file.txt", &frame);
    
    ASSERT_EQ(0, result);  /* Should fail */
}

TEST(load_frame_empty) {
    setup_test_files();
    
    Frame frame;
    int result = load_frame("test_anim/empty_frame.txt", &frame);
    
    ASSERT_EQ(1, result);  /* Should succeed even with empty file */
    
    /* All lines should be filled with spaces */
    for (int i = 0; i < TERMINAL_HEIGHT; i++) {
        ASSERT_EQ(' ', frame.content[i][0]);
        ASSERT_EQ('\0', frame.content[i][TERMINAL_WIDTH]);
    }
    
    cleanup_test_files();
}

/* Test frame structure constraints */
TEST(frame_structure_constraints) {
    Frame frame;
    
    /* Verify frame dimensions match our constants */
    ASSERT_EQ(TERMINAL_HEIGHT, sizeof(frame.content) / sizeof(frame.content[0]));
    ASSERT_EQ(TERMINAL_WIDTH + 1, sizeof(frame.content[0]));
}

/* Test constants are reasonable */
TEST(constants_sanity) {
    ASSERT_EQ(80, TERMINAL_WIDTH);
    ASSERT_EQ(24, TERMINAL_HEIGHT);
    ASSERT_TRUE(ANIMATION_DELAY > 0);
    ASSERT_TRUE(MAX_FRAMES > 0);
    ASSERT_TRUE(MAX_FILENAME_LENGTH > 10);
}

int main(void) {
    printf("Running stickman unit tests...\n");
    printf("=====================================\n");
    
    /* Note: These tests are somewhat perfunctory for now, but establish */
    /* good practices and provide a foundation for future functionality */
    
    RUN_TEST(constants_sanity);
    RUN_TEST(frame_structure_constraints);
    RUN_TEST(compare_filenames_basic);
    RUN_TEST(compare_filenames_sorting);
    RUN_TEST(load_frame_basic);
    RUN_TEST(load_frame_nonexistent);
    RUN_TEST(load_frame_empty);
    
    printf("=====================================\n");
    printf("Tests run: %d, Passed: %d\n", tests_run, tests_passed);
    
    if (tests_passed == tests_run) {
        printf("All tests PASSED!\n");
        return 0;
    } else {
        printf("Some tests FAILED!\n");
        return 1;
    }
}
