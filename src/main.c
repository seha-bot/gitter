#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "nec.h"
#include "termui.h"
#include "gitter.h"

int main(int argc, char *argv[])
{
    termui_init();
    printf(TERMUI_NOCURSOR);

    termui *root, *status;
    root = termui_box(0, 0, 0,
        status = termui_box(0, 0, 1, 0),
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

    char* branch = git_get_current_branch();
    if(branch == NULL) return 1;
    char* abc = (char*)malloc(strlen(branch) + 25);
    sprintf(abc, "Current branch: %s (clean)", branch);
    status->text = abc;

    termui_fullscreen(root);
    termui_plot(root);

    char c;
    while(termui_read(&c) != 'q')
    {
        switch(c)
        {
        case 'b':
            if(branch_selector()) return 1;
            break;
        }
        termui_plot(root);
    }

    termui_free(root);
    termui_deinit();
    printf(TERMUI_CURSOR);
    return 0;
}

