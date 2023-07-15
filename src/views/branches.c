#include "gitter.h"
#include <stdio.h>
#include "nec.h"

void branch_view(void)
{
    int selection = 0;
    while(selection != -1)
    {
        termui *root, *box;
        root = wrapper(box = termui_box(0, 0, 0,
            termui_text("  s: Switch", termui_box(0, 0, 1, 0)),
            termui_text("  n: New branch (prompt input)", termui_box(0, 0, 1, 0)),
            termui_text("  d: Delete", termui_box(0, 0, 1, 0)),
            termui_text("  q: Back", termui_box(0, 0, 1, 0)),
            termui_box(0, 1, 1, 0),
        0));

        char** branches = git_get_branches();

        for(int i = 0; i < nec_size(branches); i++)
        {
            if(i == 0)
            {
                nec_push(box->children,
                    termui_text("Branches", termui_box(0, 0, 1, 0))
                );
            }
            nec_push(box->children,
                selector(i == selection, termui_text(branches[i], termui_box(0, 0, 1, 0)))
            );
        }

        termui_fullscreen(root);
        termui_plot(root);

        char c = termui_read(0);
        if(c == 'j' && selection != nec_size(branches) - 1) selection++;
        else if(c == 'k' && selection != 0) selection--;
        else if(c == 's')
        {
            git_set_current_branch(branches[selection]);
        }
        else if(c == 'n')
        {
            char* name = input_bar();
            if(name != NULL)
            {
                if(nec_size(name) > 1)
                {
                    git_create_branch(name);
                }
                nec_free(name);
            }
        }
        else if(c == 'd')
        {
            git_delete_branch(branches[selection]);
            selection = 0;
        }
        if(c == 'q') selection = -1;

        for(int i = 0; i < nec_size(branches); i++)
        {
            nec_free(branches[i]);
        }
        nec_free(branches);
        wrapper_free(root);
        termui_free(root);
    }
}

