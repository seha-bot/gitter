#include "gitter.h"
#include <stdio.h>
#include <ctype.h>
#include "nec.h"

char* input_bar(void)
{
    printf(TERMUI_CURSOR);
    char* buffer = NULL;
    nec_push(buffer, '\0');

    while(1)
    {
        termui* root = termui_box(0, 0, 1, 0);
        termui_fullscreen(root);

        int bottom = root->bottom;
        root->top = root->bottom;
        root->drawTop = root->drawBottom;
        root->height = 1;

        root->text = buffer;
        termui_plot(root);
        termui_focus(root);

        char c = termui_read(NULL);
        if(c == '\033')
        {
            nec_free(buffer);
            break;
        }
        if(c == 13) break;
        if(c == 127 && nec_size(buffer) > 1)
        {
            nec_pop(buffer);
            buffer[nec_size(buffer) - 1] = '\0';
            continue;
        }
        if(!isalnum(c) && c != '-') continue;
        buffer[nec_size(buffer) - 1] = c;
        nec_push(buffer, '\0');
    }

    printf(TERMUI_NOCURSOR);
    return buffer;
}

