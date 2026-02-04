#include <unistd.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 

#define NUM_CHILDREN 15 

int main() {

    // Array to hold child PIDs
    pid_t childPids[NUM_CHILDREN];
    int status;

    // Counters for summary
    int normal_zero = 0;
    int normal_nonzero = 0;
    int signaled = 0;

    // Commands for each child process
    char *commands[NUM_CHILDREN][3] = { 
        {"ls", NULL, NULL}, 
        {"echo", "Hello, World!", NULL},
        {"no_cmd1", NULL, NULL}, 
        {"date", NULL, NULL}, 
        {"echo","Hello again!", NULL},  
        {"pwd", NULL, NULL},  
        {"whoami", NULL, NULL}, 
        {"uname", "-a", NULL}, 
        {"no_cmd2", NULL, NULL}, 
        {"id", NULL, NULL}, 
        {"echo", "Goodbye!", NULL}, 
        {"ls", NULL, NULL}, 
        {"echo", "This will work", NULL}, 
        {"uptime", NULL, NULL}, 
        {"sleep", NULL, NULL}  
    };
    // Print parent PID
    printf("Parent process PID: %d\n\n", getpid());

    // Create child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        // Error handling for fork
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } 
        // Child process
        if (pid == 0) { 
            printf("[Child #%d] PID=%d PID=%d about to run: %s\n", i, getpid(), getppid(), commands[i][0]); 
            
            // Intentionally cause abort for children 13 and 14
            if (i == 13 || i == 14){ 
                printf("[Child #%d] Intentionally calling abort() to demonstrate SIGABRT\n", i);
                abort();
            }
            // Execute the command
            execvp(commands[i][0], commands[i]);
            // If execvp returns, an error occurred
            perror("execvp failed");
            exit(127);
        
        // Parent process
        } else { 
            childPids[i] = pid; 
            printf("[Parent] created Child #%d with PID=%d\n", i, pid);  
        }

    }
    // Parent waits for children in creation order
    printf("\n=== Parent Waiting in CREATION ORDER with exit-status reporting ===\n\n");

    // Wait for each child and report status
    for (int i = 0; i < NUM_CHILDREN; i++) {
        waitpid(childPids[i], &status, 0);
        
        printf("[Parent] Child #%d PID=%d finished: ", i, childPids[i]);
        
        // Check how the child terminated
        if (WIFEXITED(status)){ 
            int code = WEXITSTATUS(status); 
            printf("exited normally, exit_code=%d\n", code);
            if (code == 0){
                normal_zero++;
            } else {
                normal_nonzero++;       
        }
        } else if (WIFSIGNALED(status)){ 
            printf("terminated by signal %d (abort trap: %d)\n", WTERMSIG(status), SIGABRT);
            signaled++;
        }
    }
    // Calculate total children
    int total = normal_zero + normal_nonzero + signaled;

    // Print summary
    printf("\n=== Summary ===\n");
    printf("Total children: %d\n", total);
    printf("Exited with code 0 (success): %d\n", normal_zero);
    printf("Exited with non-zero code): %d\n", normal_nonzero);
    printf("Terminated by signal: %d\n", signaled);
    return 0;
}