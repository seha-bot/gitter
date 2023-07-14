#include "gitter.h"
#include <stdio.h>
#include <string.h>
#include "nec.h"
#include "termui.h"

termui* termui_selector(const char* icon, termui* child)
{
    return termui_box(TERMUI_ROW, 0, 1,
        termui_text(icon, termui_box(0, strlen(icon), 1, 0)),
        child,
    0);
}

int branch_view(void)
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

    size_t selectedId = -1;
    char** branches = git_get_branches(&selectedId);
    if(selectedId == -1) return 1;
    char* abc = (char*)malloc(strlen(branches[selectedId]) + 25);
    sprintf(abc, "Current branch: %s (clean)", branches[selectedId]);
    status->text = abc;

    for(int i = 0; i < nec_size(branches); i++)
    {
        if(i == 0)
        {
            nec_push(root->children,
                termui_text("Branches", termui_box(0, 0, 1, 0))
            );
        }
        nec_push(root->children,
            termui_selector(">", termui_text(branches[i], termui_box(0, 0, 1, 0)))
        );
    }

    termui_fullscreen(root);
    termui_plot(root);

    while(termui_read(0) != 'q')
    {
    }

    for(int i = 0; i < nec_size(branches); i++)
    {
        nec_free(branches[i]);
    }
    nec_free(branches);
    termui_free(root);
    return 0;
}

