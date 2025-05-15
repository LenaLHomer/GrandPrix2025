#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5


/**
 * @brief Compute the gas consumption of a requested acceleration
 *
 * CAUTION: Even an illegal move will result in gas consumption. Producing
 * illegal moves should be prevented as much as possible!
 *
 * @param accX Acceleration x component
 * @param accY Acceleration y component
 * @param speedX Speed x component
 * @param speedY Speed y component
 * @param inSand (boolean)
 * @return Number of gas units consumed
 */
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand)
{
  int gas = accX * accX + accY * accY;
  gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
  if (inSand) {
    gas += 1;
  }
  return -gas;
}

char** readMapData(int height, int width) {
  int row;
  int i;
  char line_buffer[MAX_LINE_LENGTH];
  char** mapData;  /* forme mapData[height, width] */
  mapData = (char**)malloc(height*sizeof(char));

  for (row = 0; row < height; ++row) {        
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    fputs(line_buffer, stderr);

    mapData[row] = (char*)malloc(width*sizeof(char));
    for (i = 0; i < width; i++) {
      mapData[row][i] = line_buffer[i];
    }
  }
  return mapData;
}

void freeMapData(char** mapData, int height) {
  int i;

  for (i = 0; i < height; ++i) {
    free(mapData[i]);
  }
  free(mapData);
}

int pointStatus(int x, int y) {
  return 0;
  /* si dans sable, mettre bool inSand pour fonction gasConsumption */
}
int main()
{
  int row;
  int width, height;
  int gasLevel;
  int boosts = BOOSTS_AT_START;
  int round = 0;
  int accelerationX = 1, accelerationY = 0;
  int speedX = 0, speedY = 0;
  char action[100];
  char line_buffer[MAX_LINE_LENGTH];
  char** mapData;

  boosts = boosts;    /* Prevent warning "unused variable" */
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      
  
  /* Read gas level at Start */
  sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);


  fprintf(stderr, "=== >Map< ===\n");
  fprintf(stderr, "Size %d x %d\n", width, height);
  fprintf(stderr, "Gas at start %d \n\n", gasLevel);

  /* Read map data, line per line */
  /* Put the line in the buffer then display it in stderr */
  mapData = readMapData(width, height);

  fflush(stderr);
  fprintf(stderr, "\n=== Race start ===\n");
  while (!feof(stdin)) {
    int myX, myY, secondX, secondY, thirdX, thirdY;
    round++;
    fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   /* Read positions of pilots */
    sscanf(line_buffer, "%d %d %d %d %d %d",
           &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
    fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n",
            myX, myY, secondX, secondY, thirdX, thirdY);
    fflush(stderr);
    /* Gas consumption cannot be accurate here. */
    gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);
    speedX += accelerationX;
    speedY += accelerationY;
    /* Write the acceleration request to the race manager (stdout). */
    sprintf(action, "%d %d", accelerationX, accelerationY);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n", action, gasLevel);
    fflush(stderr);
    if (0 && round > 4) { /* (DISABLED) Force a segfault for testing purpose */
      int * p = NULL;
      fprintf(stderr, "Good Bye!\n");
      fflush(stderr);
      *p = 0;
    }
  }

  freeMapData(mapData, height);

  return EXIT_SUCCESS;
}
