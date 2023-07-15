#ifndef SEHA_GITTER
#define SEHA_GITTER

#include <stdlib.h>
#include "termui.h"

// Api git[_<get/set/create/delete>]_<command>
char* git_get_current_branch(void);
void git_set_current_branch(const char* name);
char** git_get_branches(void);
void git_create_branch(const char* name);
void git_delete_branch(const char* name);
void git_fetch_and_prune(void);
void git_pull(void);
void git_push(void);

// Views
void branch_view(void);

// Widgets
termui* wrapper(termui* child);
void wrapper_free(termui* root);
termui* selector(int isSelected, termui* child);
char* input_bar(void);

#endif /* SEHA_GITTER */

