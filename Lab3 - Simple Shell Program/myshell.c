#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_TOKENS 64

// Command structure to hold parsed command information
typedef struct
{
    char *argv[MAX_TOKENS];
    char *out_file;
    int append;
    char *in_file;
} Command;

int fflush(FILE *stream); 

int parse_line(char *line, Command *cmd)
{
    // Initialize the Command structure
    memset(cmd, 0, sizeof(Command));
    int argc = 0;
    char token[MAX_LINE];
    int i = 0;
    int len = strlen(line);

    while (i < len)
    {
        int ti = 0;

        // Skip spaces
        while (i < len && isspace((unsigned char)line[i]))
            i++;
        if (i >= len)
            break;

        // Output redirection
        if (line[i] == '>')
        {
            i++;

            // Check for >> (append)
            if (line[i] == '>')
            {
                // Skip second '>'
                i++;
                // mark append
                cmd->append = 1;
            }
            else
            {
                // overwrite if not append
                cmd->append = 0;
            }
            
            // Skip spaces before filename
            while (i < len && isspace((unsigned char)line[i]))
                i++;

            char filebuf[MAX_LINE];
            int fi = 0;
            
            while (i < len && !isspace((unsigned char)line[i]))
            {
                filebuf[fi++] = line[i++];
            }

            // Null-terminate the filename
            filebuf[fi] = 0;
            cmd->out_file = strdup(filebuf);
            continue;
        }

        // Input redirection
        if (line[i] == '<')
        {
            i++;  
            
            // Skip spaces before filename
            while (i < len && isspace((unsigned char)line[i]))
                i++;

            char filebuf[MAX_LINE];
            int fi = 0;

            while (i < len && !isspace((unsigned char)line[i]))
            {
                filebuf[fi++] = line[i++];
            }

            // Null terminate the filename
            filebuf[fi] = 0;
            cmd->in_file = strdup(filebuf); 
            continue;
        }

        // Quoted token
        if (line[i] == '"' || line[i] == '\'')
        {
            char quote = line[i++];
            while (i < len && line[i] != quote)
            {
                token[ti++] = line[i++];
            }
            token[ti] = 0;
            if (i < len)
                i++;
        }
        // Normal token
        else
        {
            while (i < len && !isspace((unsigned char)line[i]) && line[i] != '>' && line[i] != '<')
            {
                token[ti++] = line[i++];
            }
            token[ti] = 0;
        }

        // Add token to argv (runs for both quoted and normal tokens)
        if (ti > 0)
        {
            cmd->argv[argc++] = strdup(token);
        }
    }

    // Null-terminate the argv array
    cmd->argv[argc] = NULL;
    return argc;
}

// Check if the command is a built-in command like 'cd' or 'exit'
int is_builtin(Command *cmd)
{
    return (strcmp(cmd->argv[0], "cd") == 0 || strcmp(cmd->argv[0], "exit") == 0);
}

// Execute built-in commands like 'cd' and 'exit'
int run_builtin(Command *cmd)
{
    if (strcmp(cmd->argv[0], "cd") == 0)
    {
        char *dir = cmd->argv[1];
        if (!dir)
            dir = getenv("HOME");
        if (chdir(dir) != 0)
            perror("cd Failed");
        return 0;
    }

    if (strcmp(cmd->argv[0], "exit") == 0)
    {
        printf("Goodbye!\n");
        return 1;
    }

    return 0;
}

// Execute external commands using fork and exec
void execute_command(Command *cmd)
{
    // Fork a child process to execute the command
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    // Child process executes the command and handles output/input redirection if specified
    if (pid == 0)
    {
        // Handle input redirection
        if (cmd->in_file)
        {
            int fd_in = open(cmd->in_file, O_RDONLY);
            if (fd_in < 0)
            {
                perror("input file open failed");
                exit(1);
            }
            // Redirect stdin to the input file
            dup2(fd_in, STDIN_FILENO);  
            close(fd_in);
        }

        // Handle output redirection (append or overwrite)
        if (cmd->out_file)
        {
            int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
            int fd_out = open(cmd->out_file, flags, 0644);
            if (fd_out < 0)
            {
                perror("output file open failed");
                exit(1);
            }
            // Redirect stdout to the output file
            dup2(fd_out, STDOUT_FILENO);  
            close(fd_out);
        }

        // Execute the command using execvp
        execvp(cmd->argv[0], cmd->argv);
        perror("exec failed");
        exit(1);
    }

    // Parent waits for the child to finish
    waitpid(pid, NULL, 0);
}

int main()
{
    char line[MAX_LINE];

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        // Read a line of input from the user
        if (!fgets(line, sizeof(line), stdin))
            break;

        // Remove newline character from the input line
        line[strcspn(line, "\n")] = 0;

        // Parse the input line into a Command structure
        Command cmd;

        // If parsing fails, skip to the next iteration to prompt the user again
        if (parse_line(line, &cmd) == 0)
            continue;

        // If no command is entered, skip to the next iteration to prompt the user again
        if (cmd.argv[0] == NULL)
            continue;

        // Check if the command is a built-in command and execute it, otherwise execute it as an external command
        if (is_builtin(&cmd))
        {
            if (run_builtin(&cmd))
            {
                break; 
            }
        }
        else
        {
            // Execute external commands
            execute_command(&cmd); 
        }
    }

    return 0;
}


