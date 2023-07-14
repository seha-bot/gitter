#ifndef SEHA_GITTER
#define SEHA_GITTER

#include <stdlib.h>

// Api git_<get/set>_<command>
char* git_get_current_branch(void);
char** git_get_branches(void);

// Views
int branch_view(void);

#endif /* SEHA_GITTER */

