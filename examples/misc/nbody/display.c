#include "nbody.h"


void initDisplay()
{
  extern int displayX11;
  displayX11 = g2_open_X11(WINDOWSIZE, WINDOWSIZE);
  g2_clear_palette(displayX11);
  g2_reset_palette(displayX11);
  g2_set_background(displayX11, 1); // g2_set_background only works for X11 
  g2_pen(displayX11, 0);
}    



(int (*)(void *, void *)) ;

double xaccess(Node nod)
{
  return nod->cmx;
}


if XDIR

partition(A, 0,13, xaccess)


int partition(Node *A, int p, int r, double (*access)(Node))
{
  double x = access(A[p]);
  int i = p-1;
  int j = r+1;

  while(1)
  {
    while(access(A[j]) > x)
    {
      j--;
    }

    while(access(A[i]) < x)
    {
      i++;
    } 
    
    if (i < j)
    {
      Node nod = A[i];
      A[i] = A[j];
      A[j] = nod;
    }
    else
    {
      return j;
    }
  }
}








#include "g2.h"       /* for graphics */
#include "g2_X11.h"   /* for X11 display */
#include "g2_gd.h"    /* for gif output */




extern Node *bodies;
extern int nBodies;
extern Node root;
extern int display;
extern int clear;

extern void printTab(int n);
void printNode(Node n);
extern void printTreeInner(Node root, int level);
extern void printTree(Node root, int level);

float halfWindowSize = WINDOWSIZE/2;
float scale;

/* ------------------------------------------------------------- */
/* console output */

void printTab(int n)
{
  int i;
  for(i=0; i < n; i++)
    printf("\t");
}


void printNode(Node n)
{
  if (n->type == CLUSTER)
    printf("CLUSTER[x:%f,y:%f,m:%f]\n", n->cmx, n->cmy, n->summ);
  else
    printf("BODY[x:%f/y:%f/vx:%f/vy:%f/fx:%f/fy:%f]\n", n->cmx, n->cmy, n->vx, n->vy, n->fx, n->fy);


  //printf("[t:%d/mi:%d/x:%f/y:%f/m:%f/vx:%f/vy:%f]\n", n->type, n->medianIndex, n->cmx, n->cmy, n->summ, n->vx, n->vy);
}


void printTreeInner(Node root, int level)
{
  printTab(level);
  printNode(root);
  if (root->type == CLUSTER)
  {
    printTreeInner(root->left, level+1);
    printTreeInner(root->right, level+1);
  }
}


void printTree(Node root, int level)
{
  printTreeInner(root, 0);
}





/* ------------------------------------------------------------- */
/* graphics output */


float largest(float a, float b)
{
  if(a>b)
    return a;

  return b;
}

float lowest(float a, float b)
{
  if(a<b)
    return a;

  return b;
}


float getHighestX(Node n)
{
  if (n->type == CLUSTER)
    return largest(getHighestX(n->left), getHighestX(n->right));

  return n->cmx;
}

float getHighestY(Node n)
{
  if (n->type == CLUSTER)
    return largest(getHighestY(n->left), getHighestY(n->right));

  return n->cmy;
}

float getLowestX(Node n)
{
  if (n->type == CLUSTER)
    return lowest(getLowestX(n->left), getLowestX(n->right));

  return n->cmx;
}

float getLowestY(Node n)
{
  if (n->type == CLUSTER)
    return lowest(getLowestY(n->left), getLowestY(n->right));

  return n->cmy;
}


/* left always has lower x values than the right, because of the sorting */
/* left always has lower y values than the right, because of the sorting */

void showLinesInner(Node n, int dir, float xmin, float xmax, float ymin, float ymax)
{
  if (n->type == CLUSTER)
  {
    int newDir = (dir+1)%2;
    float avg;
    float high;
    float low;

    if (dir == XDIR)
    {
      high = getHighestX(n->left);
      low = getLowestX(n->right);
    }
    else
    {
      high = getHighestY(n->left);
      low = getLowestY(n->right);
    }
    
    avg = scale *(low+high) / 2;
    

    if (dir == XDIR)
    {
      g2_line(display, avg, ymin, avg, ymax);
      showLinesInner(n->left, newDir, xmin, avg, ymin, ymax);
      showLinesInner(n->right, newDir, avg, xmax, ymin, ymax);
    }
    else
    {
      g2_line(display, xmin, avg, xmax, avg);
      showLinesInner(n->left, newDir, xmin, xmax, ymin, avg);
      showLinesInner(n->right, newDir, xmin, xmax, avg, ymax);
    }
  }
}

void showScaleLinesInner(Node n, int dir, float xmin, float xmax, float ymin, float ymax)
{
  if (n->type == CLUSTER)
  {
    int newDir = (dir+1)%2;
    float avg;
    float high;
    float low;

    if (dir == XDIR)
    {
      high = getHighestX(n->left);
      low = getLowestX(n->right);
    }
    else
    {
      high = getHighestY(n->left);
      low = getLowestY(n->right);
    }
    
    avg = scale *(low+high) /2 + halfWindowSize;
    

    if (dir == XDIR)
    {
      g2_line(display, avg, ymin, avg, ymax);
      showScaleLinesInner(n->left, newDir, xmin, avg, ymin, ymax);
      showScaleLinesInner(n->right, newDir, avg, xmax, ymin, ymax);
    }
    else
    {
      g2_line(display, xmin, avg, xmax, avg);
      showScaleLinesInner(n->left, newDir, xmin, xmax, ymin, avg);
      showScaleLinesInner(n->right, newDir, xmin, xmax, avg, ymax);
    }
  }
}

void showLines()
{
  showScaleLinesInner(root, XDIR, 0, WINDOWSIZE, 0, WINDOWSIZE);
}


void showTreeInner(Node n)
{
  if (n->type == CLUSTER)
  {
    g2_line(display, n->left->cmx, n->left->cmy, n->right->cmx, n->right->cmy);
    g2_circle(display, n->cmx, n->cmy, n->summ/100);

    showTreeInner(n->left);
    showTreeInner(n->right);
  }
}

void showScaleTreeInner(Node n)
{
  if (n->type == CLUSTER)
  {
    g2_line(display, n->left->cmx*scale+halfWindowSize, n->left->cmy*scale+halfWindowSize, n->right->cmx*scale+halfWindowSize, n->right->cmy*scale+halfWindowSize);
    g2_circle(display, n->cmx*scale+halfWindowSize, n->cmy*scale+halfWindowSize, n->summ/10);

    showScaleTreeInner(n->left);
    showScaleTreeInner(n->right);
  }
}

void showTree()
{
  showScaleTreeInner(root);
}


void computeScale()
{
  int i;
  float size = 0.0;

  /* get the active window size of bodies */
  float tmpx,tmpy; 
  for(i=0; i < nBodies; i++)
  {
    tmpx = fabsf(bodies[i]->cmx);
    tmpy = fabsf(bodies[i]->cmy);
    size = ((size >= tmpx)? size : tmpx);
    size = ((size >= tmpy)? size : tmpy);
  }
  
  scale = FRAME/(size*2);
}


float transform(float fl)
{
  return scale*fl + halfWindowSize;
}


void showBodies()
{
  int i;
  Node nod;
  g2_string(display, 0, WINDOWSIZE-20, str);

  g2_plot_QP(display, halfWindowSize, halfWindowSize);

  for(i=0; i < nBodies; i++)
  {
    nod = bodies[i];
    g2_filled_circle(display, transform(nod->cmx), transform(nod->cmy), nod->summ);
  } 
}


void clearDisplay()
{
	g2_clear(display);
}


void closeDisplay()
{
	g2_close(display);
}
