/* CS 352 -- Micro Shell!
 *
 *   Sept 21, 2000,  Phil Nelson
 *   Modified April 8, 2001
 *   Modified January 6, 2003
 *   Modified January 8, 2017
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


/* Constants */

#define LINELEN 1024

/* Prototypes */

void processline (char *line);

char** argparse(char *line, int *argcount);

int numElements(char* line);

int evenParen(char* line);


/* Shell main */

int
main (void)
{
    char   buffer [LINELEN];
    int    len;
    int *argc;

    while (1) {

        /* prompt and get line */
        fprintf (stderr, "%% ");
        if (fgets (buffer, LINELEN, stdin) != buffer)
            break;

        /* Get rid of \n at end of buffer. */
        len = strlen(buffer);
        if (buffer[len-1] == '\n')
            buffer[len-1] = 0;

        /* Run it ... */
        processline (buffer);
        argparse(buffer,argc);



    }


    if (!feof(stdin))
        perror ("read");



    return 0;		/* Also known as exit (0); */
}




void processline (char *line)
{
    pid_t  cpid;
    int    status;

    /* Start a new process to do the job. */
    cpid = fork();
    if (cpid < 0) {
        /* Fork wasn't successful */
        perror ("fork");
        return;
    }

    /* Check for who we are! */
    if (cpid == 0) {
        /* We are the child! */
        execlp (line, line, (char *)0);
        /* execlp reurned, wasn't successful */
        perror ("exec");
        fclose(stdin);  // avoid a linux stdio bug
        exit (127);
    }

    /* Have the parent wait for child to complete */
    if (wait (&status) < 0) {
        /* Wait wasn't successful */
        perror ("wait");
    }
}

char** argparse(char* line, int* argc){
    int even = evenParen(line);
    if(even == 1){
        printf("Enter even number quotation marks" );
        printf("\n");
        main();
    }


    int elements = numElements(line);
    //*argc = elements;



    //char **args = (char **)  malloc(sizeof(char *) * (elements+1));
    char ** args = malloc((elements+1)* sizeof(char*));
    args[elements] = NULL;

    int count = 0;
    char* pos = line;

    while (*pos != '\0') {
    if(*pos == '\"') {
        pos++;
        args[count] = pos;
        while (*pos != '\"') {
            //*line = *new;
            ++pos;
            //++new;
        }
        *pos = '\0';
        pos++;
        count++;
        //elements++;


    }else if (*pos == ' ') {
            *pos++;
            }
         else {

            args[count] = pos;
            while (*pos != ' ' && *pos != '\0') {


                ++pos;
            }
            *pos = '\0';
            pos++;
            count++;


        }

    }
    //printf("%s \n", args[1]);
//    for(int i = 0; i < elements; i++){
//        removeQuotes(args[i]);
//    }



    for (int i = 0; i <elements ; i++) {
        printf("Arg %d: '%s'\n", i, args[i]);

    }

return args;
}





int numElements(char* line) {
    int elements = 0;
//char* new = line;
    while (*line != '\0') {

        if (*line == '\"') {
            line++;
            while (*line != '\"') {
                //*line = *new;
                ++line;
                //++new;
            }
            line++;
            elements++;



    }else if (*line == ' ') {
            while (*line == ' ') {
                ++line;
            }
        } else {

            while (*line != ' ' && *line != '\0') {


                ++line;
            }
            elements++;


        }

    }
    return elements;
}

int evenParen(char* line){
    int counter = 0;
    int one = 1;
    int zero = 0;

    while(*line != '\0'){
        if(*line == '\"'){
            counter++;
        }
        ++line;
    }

    if((counter % 2) == 0){
        return zero;
    }else{
        return one;
    }


}


