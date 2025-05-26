#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

#include "aStar.h"
#include "listProjet.h"
#include "heapProjet.h"
#include "follow_line.h"

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
  int gas;

  gas = accX * accX + accY * accY;
  gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
  if (inSand) {
    gas += 1;
  }
  return -gas;
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

Pos2Dint findGoal(char** mapData, int width, int height) {
    Pos2Dint goal;
    int x;
    int y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (mapData[y][x] == '=') {
                goal.x = x;
                goal.y = y;
                return goal;
            }
        }
    }
    goal.x = -1;
    goal.y = -1;
    return goal;
}

Node* buildStartNode(int x, int y, int speedX, int speedY) {
  Node* start;

  start = (Node*)malloc(sizeof(Node));
  start->x = x;
  start->y = y;
  start->direction = 0;
  start->heuristic = 0;
  start->total_cost = 0;
  start->parent = NULL;
  start->nextCell = NULL;

  return start;
}

void computeAcceleration(int myX, int myY, int speedX, int speedY, int targetX, int targetY, int* accX, int* accY) {
  int nextX = myX + speedX;
  int nextY = myY + speedY;
  int dx = targetX - nextX;
  int dy = targetY - nextY;

  if (dx > 1) dx = 1;
  if (dx < -1) dx = -1;
  if (dy > 1) dy = 1;
  if (dy < -1) dy = -1;

  *accX = dx;
  *accY = dy;
}
void freePath(Node* node) {
  while(node) {
    Node* next = node->nextCell;
    free(node);
    node = next;
  }
}

int main() {
    int width; 
    int height;
    int gasLevel;
    int row;
    int i;
    int round = 0;
    int speedX = 0;
    int speedY = 0;
    int accelerationX = 0;
    int accelerationY = 0;
    char line_buffer[MAX_LINE_LENGTH];
    char action[100];
    int x;
    int y;
    char** mapData;
    Node* grid;
    Node* start;
    Node* goal;
    Node* n;
    int found;
    List path;
    Node* p;
    InfoLine line;
    Pos2Dint next;

    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
    fprintf(stderr, "=== >Map< ===\n");
    fprintf(stderr, "Size %d x %d\n", width, height);
    fprintf(stderr, "Gas at start %d \n\n", gasLevel);

    mapData = (char**)malloc(height*sizeof(char));

    for (row = 0; row < height; ++row) {   
      mapData[row] = (char*)malloc(width*sizeof(char));
      fgets(mapData[row], MAX_LINE_LENGTH, stdin);
      fprintf(mapData[row], stderr);
    }

    grid = (Node*)malloc(width * height * sizeof(Node));

    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++) {
            n = &grid[y * width + x];
            n->x = x;
            n->y = y;
            n->id = y * width + x;
            n->g = n->h = n->f = -1;
            n->parent = NULL;
        }

    start = &grid[1 * width + 1];          /* changer dynamiquement */
    goal  = &grid[(height - 2) * width + (width - 2)];

    found = aStar(start, goal, grid, width * height, mapData, width, height);

    if (!found) {
        fprintf(stderr, "No path found.\n");
        return 1;
    }

    /* Retrace le chemin */
    path = newList();   
    p = goal;
    while (p != NULL) {
        path = addNodeInList(p, path);
        p = p->parent;
    }

    /* Utilisation de follow_line */
    initLine(start->x, start->y, goal->x, goal->y, &line);
    if (nextPoint(&line, &next, 1) > 0) {
        accelerationX = next.x - start->x - speedX;
        accelerationY = next.y - start->y - speedY;
    }

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
    free(grid);
    freeMapData(mapData, height);

    return 0;
}