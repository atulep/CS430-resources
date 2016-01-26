#include <stdlib.h>
#include <stdio.h> 
#include <sys/time.h> // needed for gettimeofday in function getMilliSeconds
#include <unistd.h>   // needed for gettimeofday in function getMilliSeconds
#include <math.h>  
#include <g2.h>  
#include <g2_X11.h>  



#define DISPLAYNUMBERS
//#define DISPLAYPOINTS


typedef struct node * Node;

struct node
{
  char type;   // 0 is internal or CLUSTER, 1 is leaf or BODY
  Node left;
  Node right;
  double cmx;  // center of mass x-coordinate
  double cmy;  // center of mass y-coordinate
  double summ; // sum of mass
  double vx;   // velocity in x-direction
  double vy;   // velocity in y-direction
  double ax;   // acceleration in x-direction
  double ay;   // acceleration in y-direction
  double minx; // next 4 fields only for CLUSTER nodes: for mininum bounding region of the cluster
  double maxx;
  double miny;
  double maxy;
};



// pvm message types
const int INITIAL_BROADCAST = 0;

// for random generation of bodies
const int RANGEXY = 250;
const int RANGEM = 10;
const int RANGEV = 100;

// to make code better readable
const int XDIR = 0;
const int YDIR = 1;
const int CLUSTER = 0;
const int BODY = 1;

// fixings
const int WINDOWSIZE = 600;
const int HALFWINDOWSIZE = 300;
const int FRAME = 500;
//const double G = 6.670E-8; // cm^3 / g * s^2   // gravitational constant
const double G = 6.672E-1; // m^3 / kg * s^2   // gravitational constant
const double DT = 1;
const double HALFDT = 0.5;
const double COLLISION = 30;
const double ALPHA = 0.95;



void memError()
{
  fprintf(stderr, "Memory allocation error\n");
  exit(127);
}


double getMilliSeconds() // returns time in milliseconds since a certain time 1970 or so
{
  struct timeval now;
  gettimeofday(&now, (struct timezone *)0);
  return (double) now.tv_sec*1000.0 + now.tv_usec/1000.0;
}


Node createNode(char type)
{
  Node n = (Node) calloc(1, sizeof(struct node)); // use calloc so everything in the structure is set to 0
  if (!n) memError();

  n->type = type;
  return n;
}
