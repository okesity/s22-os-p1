/** @file streams.h
 *  @brief Level-description interface for "Streams"
 *
 *  @author Cayden Codel (ccodel)
 */

#ifndef _STREAMS_H_
#define _STREAMS_H_

/** Maximum number of Streams levels supported by p1 game */
#define MAX_LEVELS        18

/** Maximum width for any Streams board */
#define MAX_STREAMS_WIDTH  8

/** Maximum height for any Streams board */
#define MAX_STREAMS_HEIGHT 8

/** Maximum number of distinct colors for any Streams board */
#define MAX_STREAMS_COLORS 5

// Char definitions for colored ends. Used in map definition, not for coloring.
// For actual colors, see x86/video_defines.h
#define RED_STREAM      ('R')
#define BLUE_STREAM     ('B')
#define GREEN_STREAM    ('G')
#define CYAN_STREAM     ('C')
#define MAGENTA_STREAM  ('M')

// Any grid squares without endpoints will be empty at first
#define EMPTY_SQUARE    ('_')

/** @brief A struct containing information for a single Streams level.
 *
 *  A level is defined by
 *    - a height (how many grid squares tall a level is),
 *    - a width (how many grid squares wide a level is),
 *    - a number of distrinct colors on the board (2 ends per color),
 *    - and a string where each character is a grid square.
 */
typedef struct streamslevel {
  int height;
  int width;
  int colors;
  char *board;
} streamslevel_t;

/** Array containing the streams levels to run */
extern streamslevel_t *streams_levels[MAX_LEVELS];

/** The actual number of streams levels available to play */
extern int streams_nlevels;

#endif /* _STREAMS_H_ */
