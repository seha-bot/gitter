#include "gitter.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "nec.h"

char cwd[4096];

char* read_file(const char* path)
{
    FILE* file = fopen(path, "r");
    if(file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* string = (char*)malloc(size + 1);
    fread(string, 1, size, file);
    string[size] = '\0';
    fclose(file);
    return string;
}

char* execute_command(const char* command)
{
    const size_t command_len = strlen(command);
    char* extended = malloc(command_len + 11);
    if(extended == NULL) return NULL;
    sprintf(extended, "%s > .gitter", command);

    const int status = system(extended);
    free(extended);

    if(status != 0)
    {
        unlink(".gitter");
        return NULL;
    }

    char* output = read_file(".gitter");
    unlink(".gitter");
    return output;
}

char* git_get_current_branch(void)
{
    return execute_command("git branch --show-current");
}

char** git_get_branches(void)
{
    char* command = execute_command("git branch -a");
    if(command == NULL) return NULL;
    const char* raw = command;
    char** branches = NULL;
    char* branch = NULL;

    while(*raw)
    {
        const char c = *raw++;
        if(isspace(c) && c != '\n' || c == '*') continue;
        if(c == '\n')
        {
            if(branch == NULL) continue;
            nec_push(branch, '\0');
            nec_push(branches, branch);
            branch = NULL;
            continue;
        }
        nec_push(branch, c);
    }
    if(branch != NULL)
    {
        nec_push(branch, '\0');
        nec_push(branches, branch);
    }

    free(command);
    return branches;
}

