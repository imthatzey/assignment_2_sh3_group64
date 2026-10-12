**Zeynep**
- Basic shell: the `osh>` prompt loop, reading input with `fgets`, and splitting it into words in `parse_input`
- Running commands with `fork()` and `execvp()`, with the parent waiting for the child; `exit` command
- Part 2: the `history` command, which keeps the 5 most recent commands in a circular buffer
- Changed `wait(NULL)` to `waitpid(pid, NULL, 0)` so the parent waits for the specific child it just started


**Bakhtawar**
- Background commands: detecting a trailing `&`, removing it before the command runs, and not waiting for the child
- `!!`: re-running the most recent command and printing it first
- Messages for an empty history (`No commands in history!` for `!!`, `No history yet!` for `history`)
- Accepting `Exit` and `History` as well as lowercase

**AI Use**
- We used Claude (Anthropic) for help with the input handling in `main`: reading the line with `fgets`, removing the trailing newline, and skipping empty lines.
