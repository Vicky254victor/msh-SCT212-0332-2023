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
/* Task 3: run an external program in a child process */
static void run_external(char **argv) {
    pid_t pid = fork();
    if (pid < 0) {                      /* fork failed */
        perror("fork");
        last_status = 1;
        return;
    }

    if (pid == 0) {                     /* CHILD */
        execvp(argv[0], argv);
        perror(argv[0]);                /* only reached if exec FAILED */
        _exit(127);
    }

       int status;                         /* PARENT */
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        last_status = 1;
        return;
    }

    if (WIFEXITED(status))              /* finished normally */
        last_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))       /* killed by a signal */
        last_status = 128 + WTERMSIG(status);
}
/* Task 4: cd runs inside the shell, because a child cannot change our folder */
static void builtin_cd(char **argv) {
    const char *dir = argv[1];
    if (dir == NULL)
        dir = getenv("HOME");           /* "cd" alone goes home */
    if (dir == NULL) {
        fprintf(stderr, "cd: HOME not set\n");
        last_status = 1;
        return;
    }
    if (chdir(dir) != 0) {
        perror("cd");
        last_status = 1;
    } else {
        last_status = 0;
    }
}

static void builtin_help(void) {
    printf("msh: a minimal shell\n");
    printf("Built-ins: cd [dir], help, exit\n");
    last_status = 0;
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
        if (strcmp(argv[0], "exit") == 0)
            break;
        if (strcmp(argv[0], "cd") == 0) {
            builtin_cd(argv);
            continue;
        }
        if (strcmp(argv[0], "help") == 0) {
            builtin_help();
            continue;
        }
        run_external(argv);
    }

    free(line);          /* give the memory back: no leaks */
    return 0;
}
