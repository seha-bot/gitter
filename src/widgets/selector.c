#include "gitter.h"

termui* selector(int isSelected, termui* child)
{
    if(isSelected)
    {
        return termui_box(TERMUI_ROW, 0, 1,
            termui_text(" >", termui_box(0, 2, 1, 0)),
            child,
        0);
    }
    return termui_box(TERMUI_ROW, 0, 1,
        termui_box(0, 2, 1, 0),
        child,
    0);
}

