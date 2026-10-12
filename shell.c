#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

//global variable to check if there was an ampersand after a command
int ampersand = 0;

/*
* parse_input: splits `line` into words and stores them in args.
*/
int parse_input (char *line, char *args[]){
    int i = 0;
    int background = 0;

    //if the last character is &
    if (line[strlen(line) - 1] == '&'){
        //remove the & from the line so that it doesn't affect commands
        line[strlen(line) - 1] = ' ';

        //set ampersand to 1
        ampersand = 1;
    } else {
        //set ampersand to 0 so it doesn't retain old values
        ampersand = 0;
    }

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

    return background;
}

int main (void){
    char line[MAX_LINE + 2];        /* +2: room for '\n' and '\0' */
    char *args[MAX_LINE / 2 + 1];   /* command line arguments */
    int should_run = 1;             /* flag to determine when to exit program */

    char history[5][MAX_LINE + 2];  /* the notebook, 5 lines */
    int count = 0;                  /* total commands typed so far */
    char original[MAX_LINE + 2];    /* photocopy of the typed line */

    while (should_run){
        printf("osh> ");
        fflush(stdout);

        /* -------- INPUT HANDLING --------- */

        if (fgets(line, sizeof(line), stdin) == NULL){
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {continue;}

        //if the user enters !!
        if (strcmp(line, "!!") == 0) {
            //if there are no previous commands to copy
            if (count == 0){
                printf("No commands in history!\n");
                continue;
            }

            strcpy(line, history[(count - 1) % 5]);
            strcpy(original, history[(count - 1) % 5]);
            printf("%s\n", original);
        } else {
            strcpy(original, line);
        }

        int background = parse_input(line, args);

        if (args[0] == NULL) {continue;}

        if ((strcmp(args[0], "exit") == 0) || (strcmp(args[0], "Exit") == 0)) {
            should_run = 0;
            continue;
        }

        if ((strcmp(args[0], "history") == 0) || (strcmp(args[0], "History") == 0)){
            // let them know if there's no history to show
            if (count == 0){
                printf("No history yet!\n");
            }

            for (int n = count; n >= 1 && n > count - 5; n--) {
                printf("%d %s\n", n, history[(n - 1) % 5]);
            }
            continue;
        }


        strcpy(history[count % 5], original);
        count++;

        pid_t pid = fork();

        if (pid < 0){
            perror("fork");
        } else if (pid == 0){
            execvp(args[0], args);
            perror("osh");
            exit(1);
        } else {
            if (ampersand == 0){ // only make the parent wait if there was no &
                if (background == 0) {
                    waitpid(pid, NULL, 0);
                }
            }
        }
    }
    return 0;
}
