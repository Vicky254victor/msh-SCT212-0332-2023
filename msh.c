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

int main(void) {
    char *line = NULL;   /* getline will allocate this memory for us */
    size_t cap = 0;      /* getline keeps track of how big it is */

    for (;;) {
        printf("msh[%d]> ", last_status);
        fflush(stdout);  /* make the prompt appear right now */

        if (getline(&line, &cap, stdin) == -1) {   /* Ctrl-D lands here */
            printf("\n");
            break;
        }

        printf("you typed: %s", line);   /* temporary, just for testing */
    }

    free(line);          /* give the memory back: no leaks */
    return 0;
}
