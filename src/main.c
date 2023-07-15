#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "gitter.h"
#include "nec.h"

int main(int argc, char *argv[])
{
    termui_init();
    printf(TERMUI_NOCURSOR);

    termui* root = wrapper(termui_box(0, 0, 0,
        termui_text("  f: Fetch", termui_box(0, 0, 1, 0)), // FINISHED
        termui_text("  p: Pull", termui_box(0, 0, 1, 0)), // FINISHED
        termui_text("  P: Push", termui_box(0, 0, 1, 0)), // FINISHED
        termui_text("  c: Commit", termui_box(0, 0, 1, 0)),
        termui_text("    a: Amend (toggle)", termui_box(0, 0, 1, 0)),
        termui_text("    c: Continue (prompt input for title)", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  b: Branching", termui_box(0, 0, 1, 0)), // FINISHED
        termui_text("  r: Reset (open list of commits)", termui_box(0, 0, 1, 0)),
        termui_text("    s: Soft (toggle)", termui_box(0, 0, 1, 0)),
        termui_text("    c: Continue", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  s: Status (open list of files changed)", termui_box(0, 0, 1, 0)),
        termui_text("    d: Diff (for selected file)", termui_box(0, 0, 1, 0)),
        termui_text("    q: Back", termui_box(0, 0, 1, 0)),
        termui_text("  q: Quit", termui_box(0, 0, 1, 0)), // FINISHED
    0));
    if(root == NULL)
    {
        termui_deinit();
        printf(TERMUI_CURSOR "No repository detected :(\n");
        return 1;
    }

    termui_fullscreen(root);
    termui_plot(root);

    char c;
    while(termui_read(&c) != 'q')
    {
        if(c == 'f') git_fetch_and_prune();
        else if(c == 'p') git_pull();
        else if(c == 'P') git_push();
        else if(c == 'b') branch_view();
        termui_plot(root);
    }

    wrapper_free(root);
    termui_free(root);
    termui_deinit();
    printf(TERMUI_CURSOR);
    return 0;
}

