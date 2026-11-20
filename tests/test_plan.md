# Test plan for msh

| # | Input | Expected behaviour | Actual result |
|---|---|---|---|
| 1 | (blank line) | New prompt, no error | New prompt shown, prompt stayed msh[0]> |
| 2 | echo hello world | Prints text; status 0 | Printed hello world, prompt msh[0]> |
| 3 | ls /nonexistent | Error from ls; non-zero status | ls printed its error, prompt msh[2]> |
| 4 | nosuchcommand | Error message; status 127 | Printed "nosuchcommand: No such file or directory", prompt msh[127]>, shell kept running |
| 5 | cd /tmp then pwd | Prints /tmp | pwd printed /tmp |
| 6 | sleep 3 & | Prompt returns at once; PID shown | See transcript below |
| 7 | echo hi > f.txt then cat f.txt | File contains hi | See transcript below |
| 8 | sleep 100 then Ctrl-C | Sleep stops; shell survives; status 130 | Sleep stopped, shell survived, prompt showed msh[130]> |
| 9 | Ctrl-D at the prompt | Shell exits cleanly | See transcript below (end of input ends the shell) |
| 10 | cd then pwd | Goes to the home directory | pwd printed /home/ghost |
| 11 | echo hi> (no space before >) | Redirect to file | Not recognised: printed "hi>" as text, because > must be a separate word (known limitation) |
| 12 | echo hi > (no file name) | Syntax error, status 1 | See transcript below |

## Transcript of tests 6, 7, 9 and 12 (input piped into ./msh)

~~~
msh[0]> [bg] started pid 8367
msh[0]> msh[0]> hi
msh[0]> [bg] pid 8367 finished
msh[0]> msh[0]> msh: syntax error near >
msh[1]> 
~~~
