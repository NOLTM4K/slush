#define bufferSize 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void interrupt(int interruptSignal) {
    write(0, "\nKeyboard input interrupted\nslush$ ", 35);
}

int builtin_cd(char **args) {  // run cd
    if (chdir(args[1]) != 0) {
        perror("cd error");
    }
    return 1;
}

int readAndSeparateLine(char *pointer, char *processes[], char *delim) { // separate input commands
    if (pointer == NULL) {
        return -1;
    }
    if (*pointer == '\0') {
        printf("Invalid null command\n");
        return -1;
    }
    else {
        char *toUse = malloc(strlen(pointer) + 1);
        strcpy(toUse, pointer);
        pointer = strtok(NULL, delim);
        int addUp = readAndSeparateLine(pointer, processes, delim);
        processes[++addUp] = toUse;
        return addUp;
    }
}

int main( int argc, char* argv[] ){

    
    struct sigaction action;

    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);
    signal(SIGINT, interrupt); // Ties interrupt handler function to ^C user input

    char delimOuter[] = "(";
	char delimInner[] = " ";
    char builtIn[] = "cd";
    char buffer[bufferSize];
    char* process[bufferSize];
    
    printf("Welcome to SLU shell");
    while(1) {
        printf("\nslush$ ");
        if (fgets(buffer, bufferSize, stdin) == NULL) {
            break;
        }
        
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length-1] == '\n') {
            buffer[length - 1] = '\0';
        }
        int pipefd[2];
        int lastPipe;
        char *ptr = strtok(buffer, delimOuter);
        int getProcesses = readAndSeparateLine(ptr, process, delimOuter);
        if (getProcesses != -1) {
            int j;
            for (j=0; j <= getProcesses; j++) {
                int max_args = 15;
                int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
                ptr = strtok(process[j], delimInner);
                if (ptr == NULL) {
                    printf("Invalid null command\n");
                    break;
                }
                char* cmd = ptr;
                char* my_argv[max_argv_size];
                my_argv[0] = cmd;
                int i = 1;
                while (ptr != NULL) {
                    ptr = strtok(NULL, delimInner);
                    if (ptr == NULL) {
                        my_argv[i] = '\0';
                    }
                    else {
                        my_argv[i] = ptr;
                    }
                    i++;
                }
                if (strcmp(cmd, builtIn) == 0 && my_argv[2] == '\0') {
                    if (getProcesses != 0)
                        printf("cd must appear on a line by itself");
                    else
                        builtin_cd(my_argv);
                }
                else if (getProcesses == 0) {
                    int child = fork();
                    if (child == 0) {
                        int ret = execvp(cmd, my_argv);
                        
                        if( ret == -1 ) {
                            perror(cmd);
                            exit(-1);
                        }
                    }
                    else
                        wait(NULL);
                }
                else {
                    if (getProcesses != j) {
                        pipe( pipefd );
                    }
                    int child = fork();
                    if (child == 0) {
                        signal(SIGINT, SIG_DFL);
                        // close(pipefd[0]);
                        if (j == 0) { // original rightmost command
                            dup2(pipefd[1], STDOUT_FILENO); //pipe stdout to write end if more processes follow
                            close(pipefd[0]);       // close read end
                        }
                        else if (j == getProcesses) { // original leftmost command
                            dup2(lastPipe, STDIN_FILENO);   // pipe standard in to previous read end
                        }
                        else {  // in the pipeline
                            dup2(lastPipe, STDIN_FILENO);   // pipe standard in to previous read end
                            dup2(pipefd[1], STDOUT_FILENO); // pipe standard out to write end
                            close(pipefd[0]);               // close read end
                        }
                        int ret = execvp(cmd, my_argv);
                        
                        if( ret == -1 ) {
                            perror(cmd);
                            exit(-1);
                        }
                    }
                    else if (child > 0) {       // parent
                        if (getProcesses == j) { // last command
                            close(lastPipe);    // close previous read end
                            // wait(NULL); 
                        }
                        else {
                            close(pipefd[1]);      // close write end
                            if (j > 0)
                                close(lastPipe);       // close previous
                            lastPipe = pipefd[0];  // make read end accessible in next loop
                        }
                    }
                }
            }
            int k;
            for(k=0; k <= getProcesses; k++) {
                wait(NULL);
            }
        }
    }
    
    return 0;
}
