
#ifndef __GL_DISPLAY_H
#define __GL_DISPLAY_H

#include <GL/glut.h>

#define MAX   1024  /* Maximum dimension for data array. */
#define RATE    30  /* Target frame rate. */

/* Open GL Initialization function.
 * Pass the command line arguments so they can be parsed for OpenGL options.

void glutInit(int *argc, char **argv);

 */

 
/* Call this function once to set up the output display.
 *
 * Parameters:
 *
 * - size: dimensions of the data (e.g. 512 means 512 X 512 data points).
 *         NOTE: MUST NOT EXCEED MAX!!!  Feel free to change MAX, and
 *         re-compile gldisplay.c
 *
 * - data: MAX X MAX array of data points to be plotted.
 *         NOTE: The same data array will be used by calls to display_once().
 */
void initwin(int size, char *data, int argc, char **argv);

/* Call this function before exiting, to avoid SEGMENTATION FAULT */
void closewin();


/* Call this function each time the data needs to be displayed. */
void display_once();

#endif



