#include "gitter.h"
#include <stdio.h>
#include <string.h>
#include "termui.h"

termui* wrapper(termui* child)
{
    termui *root, *status;
    root = termui_box(0, 0, 0,
        status = termui_box(0, 0, 1, 0),
        termui_box(0, 1, 1, 0),
        child,
    0);

    char* branch = git_get_current_branch();
    if(branch == NULL) return NULL;

    // TODO: replace (clean) with (dirty) if git status isn't empty
    char* text = (char*)malloc(strlen(branch) + 25);
    sprintf(text, "Current branch: %s(clean)", branch);
    status->text = text;

    return root;
}

void wrapper_free(termui* root)
{
    free((void*)root->children[0]->text);
}

