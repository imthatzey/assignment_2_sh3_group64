#include <stdio.h>
#include <unistd.h>
#include <string.h>     
#include <stdlib.h>    
#include <sys/wait.h>   

#define MAX_LINE 80 

/*
 * parse_input: splits `line` into words and stores them in args.
 */
int parse_input(char *line, char *args[])
{
    int i = 0;
    int background = 0;
    
    const char delimiters[] = " \t\n";
    
    // Get the first word (token)
    char *word = strtok(line, delimiters);
    
    // Continue gathering words until strtok returns NULL
    while (word != NULL) {
        args[i] = word;
        i++;
        word = strtok(NULL, delimiters);
    }
    args[i] = NULL;

    /* STEP 3: check for '&'.
     */

    return background;
}

int main(void)
{
    char line[MAX_LINE + 2];           /* +2: room for '\n' and '\0' */
    char *args[MAX_LINE / 2 + 1];      /* command line arguments */
    int should_run = 1;                /* flag to determine when to exit program */

    char history[5][MAX_LINE + 2];   /* the notebook, 5 lines */
    int count = 0;                   /* total commands typed so far */
    char original[MAX_LINE + 2];     /* photocopy of the typed line */

    while (should_run) {
        printf("osh>");
        fflush(stdout);

        /* ---------- INPUT HANDLING ---------- */

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) { continue; }

        strcpy(original, line); 

        int background = parse_input(line, args);

        if (args[0] == NULL) { continue; }

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }
        
        if (strcmp(args[0], "history") == 0) {
            for (int n = count; n >= 1 && n > count - 5; n--) {
                printf("%d %s\n", n, history[(n - 1) % 5]);
            }
            continue;
        }

        strcpy(history[count % 5], original);
        count++;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("osh");
            exit(1);
        } else {
            if (background == 0) {
                wait(NULL);
            }
        }

    }
    return 0;
}