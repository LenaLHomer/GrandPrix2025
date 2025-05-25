#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

#include "astar.h"
#include "ListProjet.h"
#include "HeapProjet.h"
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

Pos2Dint findGoal(char** mapData, int width, int height) {
    Pos2Dint goal;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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
  Node* start = (Node*)malloc(sizeof(Node));
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
    int width, height, gasLevel;
    int speedX = 0, speedY = 0;
    int accelerationX = 0, accelerationY = 0;
    char line_buffer[MAX_LINE_LENGTH];
    char action[100];

    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
    char** mapData = readMapData(height, width);

    Node* grid = (Node*)malloc(width * height * sizeof(Node));
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            Node* n = &grid[y * width + x];
            n->x = x;
            n->y = y;
            n->id = y * width + x;
            n->g = n->h = n->f = -1;
            n->parent = NULL;
        }

    connectNeighbors(grid, width, height, mapData);

    Node* start = &grid[1 * width + 1];          // ← changer dynamiquement
    Node* goal  = &grid[(height - 2) * width + (width - 2)];

    int found = AStar(start, goal, grid, width * height);

    if (!found) {
        fprintf(stderr, "No path found.\n");
        return 1;
    }

    // Retrace le chemin
    List path = NULL;
    Node* p = goal;
    while (p != NULL) {
        NodeinList(p, &path);
        p = p->parent;
    }

    // Utilisation de follow_line
    InfoLine line;
    initLine(start->x, start->y, goal->x, goal->y, &line);
    Pos2Dint next;
    if (nextPoint(&line, &next, 1) > 0) {
        accelerationX = next.x - start->x - speedX;
        accelerationY = next.y - start->y - speedY;
    }

    // Simule 1 tour pour test
    speedX += accelerationX;
    speedY += accelerationY;
    gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);
    sprintf(action, "%d %d", accelerationX, accelerationY);
    printf("%s", action); fflush(stdout);

    fprintf(stderr, "Action: %s  | Gas: %d\n", action, gasLevel);

    free(grid);
    freeMapData(mapData, height);
    return 0;
}