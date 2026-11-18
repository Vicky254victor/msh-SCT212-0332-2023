#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

static int last_status = 0;
#define MAX_ARGS 64

/* Cut the line into words. Returns how many words were found. */
static int parse(char *line, char **argv) {
    int argc = 0;
    char *tok = strtok(line, " \t\n");
    while (tok != NULL && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;      /* argv must end with NULL */
    return argc;
}
int main(void) {
    char *line = NULL;   /* getline will allocate this memory for us */
    size_t cap = 0;      /* getline keeps track of how big it is */
    char *argv[MAX_ARGS];
    for (;;) {
        printf("msh[%d]> ", last_status);
        fflush(stdout);  /* make the prompt appear right now */

        if (getline(&line, &cap, stdin) == -1) {   /* Ctrl-D lands here */
            printf("\n");
            break;
        }

                int argc = parse(line, argv);
        if (argc == 0) continue;         /* blank line: just show a new prompt */

        for (int i = 0; i < argc; i++)   /* temporary, just for testing */
            printf("arg %d = %s\n", i, argv[i]);
    }

    free(line);          /* give the memory back: no leaks */
    return 0;
}
