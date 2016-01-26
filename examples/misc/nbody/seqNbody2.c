#include "seqNbody.h"


int nPositions;
double *oldPositions; // holds the transformed positions of the last step in x1, y1, x2, y2, ... order
double *newPositions; // holds the "real" positions of the current step in x1, y1, x2, y2, ... order
int displayX11;
double scale;
int nBodies; // number of bodies
Node *bodies;
char str[100];



void printBodies()
{
  int i;
  for (i=0; i < nBodies; i++)
  {
    Node n = bodies[i];
    fprintf(stderr, "BODY %d [x:%g/y:%g/vx:%g/vy:%g/ax:%g/ay:%g]\n", i, n->cmx, n->cmy, n->vx, n->vy, n->ax, n->ay);
  }
}


void calcForce(Node nod)
{
  int i;
  for(i=0; i < nBodies; i++)
  {
    Node b = bodies[i];

    if (nod != b) // do not!!! calculate a force of a body on itself
    {
      double rx = b->cmx - nod->cmx;
      double ry = b->cmy - nod->cmy;

      double r2 = rx*rx + ry*ry;
      double r  = sqrt(r2); // this is the distance between the body nod and the body b
	
      if (r > COLLISION) // if not too close
      {
	double part = G * (b->summ) / r2;

	nod->ax += part*rx/r;
	nod->ay += part*ry/r;

	//fprintf(stderr, "calcForce: ax:%g, ay:%g, part:%g, r2:%g, r:%g, rx:%g, ry:%g\n", nod->ax, nod->ay, part, r2, r, rx, ry);
      }
      else // there is a collision
      {
	// we do not calculate the forces between those two bodies
      }
    }
  }
}


void initDisplay()
{
  displayX11 = g2_open_X11(WINDOWSIZE, WINDOWSIZE);
  system("sleep 0.43");
  g2_set_background(displayX11, 1); // g2_set_background only works for X11 
  g2_pen(displayX11, 0);
  g2_plot_QP(displayX11, HALFWINDOWSIZE, HALFWINDOWSIZE);
}
   
 
void computeScale()
{
  int i;
  double size = 0.0; // the highest of all absolute x and y values

  /* get the active window size of bodies */
  double tmp; 
  for(i=0; i < nPositions; i++)
  {
    tmp = fabs(newPositions[i]);
    size = (size >= tmp)? size : tmp;
  }
  
  scale = FRAME/(size*2);
  //scale = 1; // that disables the scaling
}


double transform(double coordinate)
{
  return scale*coordinate + HALFWINDOWSIZE;
}


void displayUpdatedBodies()
{
  computeScale(); // compute the scale based on the new positions

  // delete the old scale factor
  g2_pen(displayX11, 1); 
  g2_filled_rectangle(displayX11, 0, WINDOWSIZE-20, 200, WINDOWSIZE); 
  g2_pen(displayX11, 0); 

  // display the new scale factor
  sprintf(str, "scaleFactor is %g", scale);
  g2_string(displayX11, 0, WINDOWSIZE-20, str);
  g2_plot_QP(displayX11, HALFWINDOWSIZE, HALFWINDOWSIZE);

  // display the bodies in their updated positions
  int i;
  for(i=0; i < nPositions; i++)
  {
    // overwrite the old body display
    g2_pen(displayX11, 1);
#ifdef DISPLAYPOINTS
    g2_plot_QP(displayX11, oldPositions[i], oldPositions[i+1]);
#else
    sprintf(str, "%d", i);
    g2_string(displayX11, oldPositions[i], oldPositions[i+1], str);
#endif
    g2_pen(displayX11, 0);

    // transform the new position coordinates and display the body
    double x = transform(newPositions[i]);
    oldPositions[i] = x;
    i++;
    double y = transform(newPositions[i]);
    oldPositions[i] = y;
      
#ifdef DISPLAYPOINTS
    g2_plot_QP(displayX11, x, y);
#else
    g2_string(displayX11, x, y, str);
#endif
  }
  
  //system("sleep 0.05");
}



int main(int argc, char **argv)
{
  int i, j, k, s; // running variables

  // check if usage is correct

  if (argc != 5) 
  {
    fprintf(stderr, "Usage: %s <no bodies> <no steps> <seed> <rotate>\n", argv[0]);
    exit(1);
  } 


  // process command line parameters

  nBodies = atoi(argv[1]);     // number of bodies
  int nsteps = atoi(argv[2]);  // number of steps 
  int seed = atoi(argv[3]);    // seed for the random number genearator
  int rotate = atoi(argv[4]);  // 0 no, 1 yes

  if (nsteps <= 0)
  {
    fprintf(stderr, "<no steps> must be bigger than 0\n");
    exit(1);
  }


  // Get initial data

  nPositions = 2 * nBodies;

  oldPositions = (double *) malloc(nPositions * sizeof(double));
  if (!oldPositions) memError();

  newPositions = (double *) malloc(nPositions * sizeof(double));
  if (!newPositions) memError();

  double *initDataArray = (double *) malloc(nBodies * 5 * sizeof(double));
  if (!initDataArray) memError();

  srandom(seed);

  j = 0;
  k = 0;
  for(i=0; i < nBodies; i++)
  {
    initDataArray[j] = random()%RANGEXY; 
    if (i%2) initDataArray[j] = -initDataArray[j];
    newPositions[k] = initDataArray[j];
    j++;
    k++;

    initDataArray[j] = random()%RANGEXY; 
    if (i%4 > 1) initDataArray[j] = -initDataArray[j];
    newPositions[k] = initDataArray[j];
    j++;
    k++;

    initDataArray[j] = random()%RANGEM + 1; 
    j++;
    initDataArray[j] = random()%RANGEV; 
    if (rotate)
    {
      if(initDataArray[j-2] < 0)
	initDataArray[j] = -initDataArray[j];
    }
    else
    {
      if (i%2) 
	initDataArray[j] = -initDataArray[j];
      initDataArray[j] = 0; // no initial velocities at the moment if no rotation
    }
    j++;
    initDataArray[j] = random()%RANGEV; 
    if (rotate)
    {
      if(initDataArray[j-4] > 0)
	initDataArray[j] = -initDataArray[j];
    }
    else
    {
      if (i%4 > 1) 
	initDataArray[j] = -initDataArray[j];
      initDataArray[j] = 0; // no initial velocities at the moment if no rotation
    }
    j++;
  }

  if (rotate) // put a very big mass in the middle
  {
    initDataArray[0] = 0;
    newPositions[0] = initDataArray[0];
    initDataArray[1] = 0;
    newPositions[1] = initDataArray[1];
    initDataArray[2] = rotate;
    initDataArray[3] = 0;
    initDataArray[4] = 0;
  }

  /*
  initDataArray[0] = -200;
  newPositions[0] = initDataArray[0];
  initDataArray[1] = 0;
  newPositions[1] = initDataArray[1];
  initDataArray[2] = 200;
  initDataArray[3] = 0;
  initDataArray[4] = 0;
  
  initDataArray[5] = 200;
  newPositions[2] = initDataArray[5];
  initDataArray[6] = 0;
  newPositions[3] = initDataArray[6];
  initDataArray[7] = 200;
  initDataArray[8] = 0;
  initDataArray[9] = 0;
  */  

  fprintf(stderr, "End of: Get initial data \n");


  // Display bodies in their initial positions

  initDisplay();
  computeScale();

  for(i=0; i < nPositions; i++)
  {
#ifdef DISPLAYNUMBERS
    sprintf(str, "%d", i);
#endif
    double x = transform(newPositions[i]);
    oldPositions[i] = x;
    i++;
    double y = transform(newPositions[i]);
    oldPositions[i] = y;

    //fprintf(stderr, "x = %g, y = %g\n", x, y);

#ifdef DISPLAYPOINTS
    g2_plot_QP(displayX11, x, y);
#else
    g2_string(displayX11, x, y, str);
#endif
  }

  fprintf(stderr, "End of: Display bodies in their initial positions \n");


  // Create the bodies array

  bodies = (Node *) malloc(nBodies * sizeof(Node));
  if (!bodies) memError();

  j = 0;
  for(i=0; i < nBodies; i++)
  {
    Node nod = createNode(BODY);

    nod->cmx = initDataArray[j]; j++;
    nod->cmy = initDataArray[j]; j++;
    nod->summ = initDataArray[j]; j++;
    nod->vx = initDataArray[j]; j++;
    nod->vy = initDataArray[j]; j++;

    bodies[i] = nod;
  }

  free(initDataArray);
  //printBodies();

  
  double totalTime = 0;


  // Starting the first calculations with the initialData (can be seen as step 1)

  s = 1;

  fprintf(stderr, "Starting step %d\n", s);

  double startStep = getMilliSeconds();

  // calculate the new forces, velocities and positions
  j = 0;
  for(i=0; i < nBodies; i++)
  {
    Node nod = bodies[i];
    
    nod->ax = 0; // set the forces back to 0
    nod->ay = 0;
    calcForce(nod); // calculate the new force
    
    // update position and velocity of the body nod
    nod->vx += nod->ax * HALFDT; // start with leapfrog step
    nod->vy += nod->ay * HALFDT;
    nod->cmx += nod->vx * DT;
    nod->cmy += nod->vy * DT;

    newPositions[j] = nod->cmx; j++;
    newPositions[j] = nod->cmy; j++;
  }

  double timeForThisStep = getMilliSeconds()-startStep;
  totalTime += timeForThisStep;

  // display the updated positions for this first step
  displayUpdatedBodies();


  // start the following steps

  for(s=2; s <= nsteps; s++) // for each step
  {
    fprintf(stderr, "Starting step %d\n", s);
    
    startStep = getMilliSeconds();

    // calculate the new forces, velocities and positions
    j = 0;
    for(i=0; i < nBodies; i++)
    {
      Node nod = bodies[i];
      
      nod->ax = 0; // set the forces back to 0
      nod->ay = 0;
      calcForce(nod); // calculate the new force
      
      // update position and velocity of the body nod
      nod->vx += nod->ax * DT;
      nod->vy += nod->ay * DT;
      nod->cmx += nod->vx * DT;
      nod->cmy += nod->vy * DT;
      
      newPositions[j] = nod->cmx; j++;
      newPositions[j] = nod->cmy; j++;
    }
    
    timeForThisStep = getMilliSeconds()-startStep;
    totalTime += timeForThisStep;

    // display the updated positions for this first step
    displayUpdatedBodies();
  }


  // show timings

  printf("\n%d bodies, %d steps: totalTime = %g\n", nBodies, nsteps, totalTime);


  // leave display open until user hits enter

  printf("\nDone with the simulation. Press ENTER!\n");
  getchar();

  g2_close(displayX11);


  // Deallocate memory

  free(newPositions);
  free(oldPositions);



  exit(0);
}
