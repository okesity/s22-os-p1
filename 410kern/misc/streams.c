/** @file streams.c
 *  @brief Provided levels for "Streams."
 *
 *  @author Cayden Codel (ccodel)
 */

#include "streams.h"

streamslevel_t level1 = {4, 4, 3, "\
____\
__RG\
_RB_\
GB__"};

streamslevel_t level2 = {5, 7, 3, "\
GB_____\
___R___\
_BG____\
_R_____\
_______"};

streamslevel_t level3 = {8, 8, 5, "\
_______C\
M____C__\
__G_M___\
_B____R_\
_G__B___\
________\
_R______\
________"};

streamslevel_t *streams_levels[] = {&level1, &level2, &level3, 0, 0, 0};
int streams_nlevels = 3;
