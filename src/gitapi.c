#include "gitter.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "nec.h"

char commandBuffer[4096];

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

char* execute_command(const char* command, int autoFree)
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

    if(autoFree) return NULL;

    char* output = read_file(".gitter");
    unlink(".gitter");
    return output;
}

char* git_get_current_branch(void)
{
    return execute_command("git branch --show-current", 0);
}

void git_set_current_branch(const char* name)
{
    sprintf(commandBuffer, "git switch %s", name);
    execute_command(commandBuffer, 1);
}

char** git_get_branches(void)
{
    char* command = execute_command("git branch -a", 0);
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

void git_create_branch(const char* name)
{
    sprintf(commandBuffer, "git branch %s && git switch %s", name, name);
    execute_command(commandBuffer, 1);
}

void git_delete_branch(const char* name)
{
    sprintf(commandBuffer, "git branch -D %s", name);
    execute_command(commandBuffer, 1);
}

void git_fetch_and_prune(void)
{
    execute_command("git fetch origin && git remote prune origin", 1);
}

void git_pull(void)
{
    execute_command("git pull", 1);
}

void git_push(void)
{
    execute_command("git push", 1);
}

