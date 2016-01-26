
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "gldisplay.h"

#include <unistd.h>

/* If INVERT is defined, data is displayed from top to bottom.
 * Otherwise it is displayed from bottom to top.
 */
#define INVERT

enum flagValues {
	NONE   = 0,
	UPDATE = 1,
	QUIT   = 2
};

static int   _size;
static int   _total;
static char *_data;
static enum flagValues flags;
int global_argc;
char **global_argv;

static sem_t buff_lock;
static unsigned char buff_fur1[MAX * MAX];

/***********************************************************
 ******************   PRIVATE FUNCTIONS   ******************
 ***********************************************************/
#define COLORMAP_SIZE  256

static unsigned char *buff_fur2;
static float color_map[COLORMAP_SIZE][3];

void display() {
	int x, y;
	int min, max, range;
	unsigned char *buff_pos1, *buff_pos2, *buff_max;

	glClear(GL_COLOR_BUFFER_BIT);

	sem_wait(&buff_lock);

	if (flags & UPDATE) {
		memcpy(buff_fur2, buff_fur1, _size*_size*sizeof(char));

		flags &= ~UPDATE;  /* Clear the update flag. */
	}
	sem_post(&buff_lock);

	buff_pos1 = buff_fur2;
	buff_max = buff_fur2 + _total;

	for (x=y=0; buff_pos1 < buff_max; buff_pos1++) {
		glColor3fv(color_map[*buff_pos1]);
		glRecti(x, y, x+1, y+1);

		y += (++x) / _size;
		x %= _size;
	}

	glutSwapBuffers();
}

static void createColorMap() {
	int i;
	float r, g, b;
	float incr = 1.0 / 64;

	g = b = 0.0;
	r = 1.0;

	for (i=0; i<64; i++) {
		g += incr;
		color_map[i][0] = 1.0;
		color_map[i][1] = g;
		color_map[i][2] = 0.0;
	}

	for (; i<128; i++) {
		r -= incr;
		color_map[i][0] = r;
		color_map[i][1] = 1.0;
		color_map[i][2] = 0.0;
	}

	for (; i<192; i++) {
		b += incr;
		color_map[i][0] = 0.0;
		color_map[i][1] = 1.0;
		color_map[i][2] = b;
	}

	for (; i<256; i++) {
		g -= incr;
		color_map[i][0] = 0.0;
		color_map[i][1] = g;
		color_map[i][2] = 1.0;
	}
}

static int window;

static void checkFlags(int wait) {
	glutTimerFunc(wait, checkFlags, wait);

	switch (flags) {

		case UPDATE:
			glutPostRedisplay();
			break;

		case QUIT:
			flags = NONE;
			glutDestroyWindow(window);
			pthread_exit(NULL);
			break;

		case NONE:
			break;

		default:
			fprintf(stderr, "Error:  flag code unknown: %d\n", (int) flags);
			flags = NONE;
	}
}

static void *init(void *params) {
	int argc;
	char **argv;
	int win_size;
	int wait = 500 / RATE;

	if (!(win_size = _size)) {
		fprintf(stderr, "No data to display!!!\n");
		exit(1);
	}

	/* Try to keep the window from being too large. */
	while ( (win_size > 1024) )
		win_size >>= 1;

	/* Keep the window from being too small. */
	while (win_size < 128)
		win_size <<= 1;

	buff_fur2 = (char *) calloc (_total, sizeof(char));
	
	glutInit(&global_argc, global_argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(win_size, win_size);
	window = glutCreateWindow("Wave Simulation.");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _size, 0, _size);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* Disable the following events.  (We don't care about them). */
	glutEntryFunc(NULL);          /* Mouse enters or leaves window. */
	glutMenuStatusFunc(NULL);     /* Menu opened or closed. */
	glutMotionFunc(NULL);         /* Mouse moving with button pressed */
	glutPassiveMotionFunc(NULL);  /* Mouse moving w/out button pressed.*/
	glutMouseFunc(NULL);          /* Mouse button pressed or released. */

	/* Set the primary display function. */
	glutDisplayFunc(display);

	/* Set up the timer */
	glutTimerFunc(wait, checkFlags, wait);

	createColorMap();
	glutMainLoop();
}

/***********************************************************
 ******************   PUBLIC  FUNCTIONS   ******************
 ***********************************************************/

pthread_t thread_id;

void initwin(int size, char *data, int argc, char **argv) {

	if (size > MAX) {
		fprintf(stderr, "ERROR: initwin: size greater than maximum.\n");
		exit(1);
	}

	_data  = data;
	_size  = size;
	_total = size * size;
	global_argc = argc;
	global_argv = argv;

	flags = NONE;

	sem_init(&buff_lock, 0, 1);
	pthread_create(&thread_id, NULL, init, NULL);
}

void closewin() {
	flags = QUIT;
	pthread_join(thread_id, NULL);
}

void display_once() {
	int i;
	char *buff_pos1, *buff_pos2;

	/* Make sure the buffer is not currently in use. */
	sem_wait(&buff_lock);

	/* Copy the data to the shared buffer, and flag as updated. */
	buff_pos1 = buff_fur1;
	buff_pos2 = _data;
#ifdef INVERT
	buff_pos2 += (_size-1) * MAX;
#endif
	for (i=0; i<_size; i++) {
		memcpy(buff_pos1, buff_pos2, _size * sizeof(char));
		buff_pos1 += _size;
#ifdef INVERT
		buff_pos2 -= MAX;
#else
		buff_pos2 += MAX;
#endif
	}

	/* Flag the data as updated. */
	flags = UPDATE;

	/* Release the buffer. */
	sem_post(&buff_lock);
}

/* vim: set ts=3: */


