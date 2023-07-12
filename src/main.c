#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "nec.h"
#include "str.h"
#include "termui.h"

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

char** parse_branches(const char* raw, size_t* selectedId)
{
    char** branches = NULL;
    char* branch = NULL;
    int isSelected = 0;

    while(*raw)
    {
        const char c = *raw++;
        if(c == '*')
        {
            isSelected = 1;
            continue;
        }
        if(isspace(c) && c != '\n') continue;
        if(c == '\n')
        {
            if(branch == NULL) continue;
            nec_push(branch, '\0');
            nec_push(branches, branch);
            branch = NULL;
            if(isSelected)
            {
                *selectedId = nec_size(branches) - 1;
                isSelected = 0;
            }
            continue;
        }
        nec_push(branch, c);
    }
    if(branch != NULL)
    {
        nec_push(branch, '\0');
        nec_push(branches, branch);
    }

    return branches;
}

void get_branches(char*** local, char*** remote, size_t* selectedId)
{
    char* output = execute_command("git branch");
    if(output == NULL) return;
    *local = parse_branches(output, selectedId);
    free(output);

    output = execute_command("git branch -r");
    if(output == NULL) return;
    *remote = parse_branches(output, NULL);
    free(output);
}

termui* termui_selector(const char* icon, termui* child)
{
    return termui_box(TERMUI_ROW, 0, 1,
        termui_text(icon, termui_box(0, strlen(icon), 1, 0)),
        child,
    0);
}

int branch_selector(void)
{
    termui *root, *status;
    root = termui_box(0, 0, 0,
        status = termui_box(0, 0, 1, 0),
        termui_box(0, 1, 1, 0),
        termui_text("  n: New branch (prompt input)", termui_box(0, 0, 1, 0)),
        termui_text("  d: Delete branch (selected one)", termui_box(0, 0, 1, 0)),
        termui_text("  q: Back", termui_box(0, 0, 1, 0)), // FINISHED
        termui_box(0, 1, 1, 0),
    0);

    char **local, **remote;
    size_t selectedId = -1;
    get_branches(&local, &remote, &selectedId);

    if(selectedId == -1) return 1;
    char* abc = (char*)malloc(strlen(local[selectedId]) + 25);
    sprintf(abc, "Current branch: %s (clean)", local[selectedId]);
    status->text = abc;

    for(int i = 0; i < nec_size(local); i++)
    {
        if(i == 0)
        {
            nec_push(root->children,
                termui_text("Local branches", termui_box(0, 0, 1, 0))
            );
        }
        nec_push(root->children,
            termui_selector(">", termui_text(local[i], termui_box(0, 0, 1, 0)))
        );
    }
    for(int i = 0; i < nec_size(remote); i++)
    {
        if(i == 0)
        {
            nec_push(root->children, termui_box(0, 1, 1, 0));
            nec_push(root->children,
                termui_text("Remote branches", termui_box(0, 0, 1, 0))
            );
        }
        nec_push(root->children,
            termui_text(remote[i], termui_box(0, 0, 1, 0))
        );
    }

    termui_fullscreen(root);
    termui_plot(root);

    while(termui_read(0) != 'q')
    {
    }

    for(int i = 0; i < nec_size(local); i++)
    {
        nec_free(local[i]);
    }
    nec_free(local);
    for(int i = 0; i < nec_size(remote); i++)
    {
        nec_free(remote[i]);
    }
    nec_free(remote);
    termui_free(root);
    return 0;
}

int main(int argc, char *argv[])
{
    termui_init();
    printf(TERMUI_NOCURSOR);

    termui* root = termui_box(0, 0, 0,
        termui_text("Current branch: main (clean)", termui_box(0, 0, 1, 0)),
        termui_box(0, 1, 1, 0),
        termui_text("  f: Fetch", termui_box(0, 0, 1, 0)),
        termui_text("  p: Pull", termui_box(0, 0, 1, 0)),
        termui_text("  P: Push", termui_box(0, 0, 1, 0)),
        termui_text("  c: Commit", termui_box(0, 0, 1, 0)),
        termui_text("    a: Amend (toggle)", termui_box(0, 0, 1, 0)),
        termui_text("    c: Continue (prompt input for title)", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  b: Branching", termui_box(0, 0, 1, 0)),
        termui_text("    n: New branch (prompt input)", termui_box(0, 0, 1, 0)),
        termui_text("    d: Delete branch (selected one)", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  r: Reset (open list of commits)", termui_box(0, 0, 1, 0)),
        termui_text("    s: Soft (toggle)", termui_box(0, 0, 1, 0)),
        termui_text("    c: Continue", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  s: Status (open list of files changed)", termui_box(0, 0, 1, 0)),
        termui_text("    d: Diff (for selected file)", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  q: Quit", termui_box(0, 0, 1, 0)), // FINISHED
    0);

    termui_fullscreen(root);
    termui_plot(root);

    char c;
    while(termui_read(&c) != 'q')
    {
        switch(c)
        {
        case 'b':
            branch_selector();
            break;
        }
        termui_plot(root);
    }

    termui_free(root);
    termui_deinit();
    printf(TERMUI_CURSOR);
    return 0;
}

