# msh: a minimal Unix shell

Name: Victor Munene Njeru
Student ID: SCT212-0332/2023

## Build and run

    make
    ./msh

## Features implemented

| Task | Description | Status |
|---|---|---|
| 1 | Prompt, getline, clean exit on Ctrl-D | complete |
| 2 | Splitting a line into argv with strtok | complete |
| 3 | fork / execvp / waitpid | complete |
| 4 | Built-ins cd, help and exit | complete |
| 5 | Exit status shown in the prompt | complete |
| 6 | Background jobs with & | complete |
| 7 | Output redirection with > | complete |
| 8 | Ctrl-C does not kill the shell | complete |

## Why cd cannot be an external program

An external command runs in a child process, which is a copy of the shell. A directory change only affects the process that makes it, so if cd ran in a child, only that child would move. The child then exits and the parent shell stays in the old folder. So cd has to be done by the shell itself with chdir().

## Design explanation

When I press Enter, my shell reads the line with getline and splits it into words with strtok. If the first word is a built-in, the shell handles it itself. Otherwise run_external calls fork(), which makes a copy of the shell. In the child, fork returns 0, so the child calls execvp(), which replaces the copy with the program I asked for. If execvp fails, the child prints the error with perror and exits with 127. In the parent, fork returns the child's PID, so the parent calls waitpid() on that PID and waits until the child finishes. Then it reads the result with WIFEXITED and WEXITSTATUS, or with WIFSIGNALED and 128 plus the signal number, and shows it in the next prompt.

cd and exit are built-ins because they have to change the shell itself. cd changes the working directory of the process that calls chdir(), and exit has to stop the shell, so neither could work from a child that disappears afterwards.

For background jobs, a trailing & is removed from argv, and the parent skips waitpid, prints the child's PID and returns to the prompt. To avoid zombies, I call waitpid(-1, &st, WNOHANG) in a loop before every prompt. WNOHANG makes it return at once if no child has finished, so it never blocks, and every finished background child is collected there.

Output redirection is done only in the child: it opens the file, uses dup2 to point standard output at it, and closes the extra descriptor before execvp, so the shell's own output is not affected. The shell installs a SIGINT handler that only prints a newline. Because execvp resets handled signals to their default, Ctrl-C stops the foreground child but not the shell.

## Known limitations

- Only > is supported (no <, no >>, no pipes, no quotes).
- The > must be a separate word, so echo hi>f.txt is not recognised.
- A finished background job is reported when the next prompt appears, so the message waits until I press Enter.
- Pressing Ctrl-C at an empty prompt moves to a new line without redrawing the prompt.
- A line can have at most 63 words.

## Sources and AI-tool declaration

- The assignment PDF, and the man pages for fork, execvp, waitpid, dup2 and sigaction.
- Claude (Anthropic's AI assistant): I used it for step-by-step guidance, help with the code, fixing compiler errors, drafting this README and fixing my git setup. I ran and tested the code myself.
