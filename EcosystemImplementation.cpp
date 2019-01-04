#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "EcosystemInterface.h"
#include "PlantInterface.h"


/* -------------------------------------- class Tile Implementation ------------------------------------- */
Tile::Tile() {
  ground = EMPTY;
}

Tile::~Tile() {
  //cout << "Tile destructed" << endl;
}

char Tile::GetGround() {
  return ground;
}

void Tile::SetGround(char category) {
  ground = category;
}

/* ----------------------------------- class Ecosystem Implementation ----------------------------------- */

Ecosystem::Ecosystem(int size, string season) {
  terrain_size = size;
  current_season = season;

  terrain_grid = new Tile *[terrain_size];
   for(int i = 0; i < terrain_size; i++) {
      terrain_grid[i] = new Tile[terrain_size];
   }

   MapGenerator();
}

Ecosystem::~Ecosystem() {

  for(int i = 0; i < terrain_size; i++) {
    delete [] terrain_grid[i];
  }
  delete [] terrain_grid;

  cout << "Ecosystem destroyed. End of the simulation!" << endl;
}

void Ecosystem::MapGenerator() {
  int river_tiles = GenerateRiver();
  int lake_tiles = GenerateLake();
  int hill_tiles = GenerateHills();
  int meadow_tiles = GenerateMeadow();
}

int Ecosystem::GenerateRiver() {

  unsigned int no_tiles = 0;
  bool stop = false;
  int x, y, start_y;

  x = 0;
  y = (rand() % (terrain_size - 5 )) + 5;
  start_y = y;
  terrain_grid[x][y].SetGround(WATER_TILE);
  no_tiles++;

  while(true) {
    /* Before go one field further, check either if we should continue or not */
    if((y == 0 && start_y != 0)  || (y == terrain_size - 1 && start_y !=  terrain_size - 1) ||  x == terrain_size -1 ) {
      break;
    }
    /* make one step down at a time */
    x++;
    terrain_grid[x][y].SetGround(WATER_TILE);
    no_tiles++;
    /* Check the possiblity of river 's diversion */
    if(rand() % 101 < DIVERSION_FACTOR) {
      int direction = rand() % 2;
      int no_fields = rand() % 2 + 1;
      if(direction == RIGHT) {

        for(int i = 0; i < no_fields; i++) {
          if((y == 0 && start_y != 0)  || (y == terrain_size - 1 && start_y !=  terrain_size - 1) ||  x == terrain_size -1 ) {
            stop = true;
            break;
          }
          y++;
          terrain_grid[x][y].SetGround(WATER_TILE);
          no_tiles++;
        }
      } else if(direction == LEFT) {
        for(int i = 0; i < no_fields; i++) {
          if(y == 0 || y == terrain_size - 1 || x == terrain_size -1 ) {
            stop = true;
            break;
          }
          y--;
          terrain_grid[x][y].SetGround(WATER_TILE);
          no_tiles++;
        }
      }
    }
    /* Stop declares if we have reached to end of river's creation */
    if(stop == true) {
      break;
    }
  }

  return no_tiles;
}

int Ecosystem::GenerateLake() {
  int no_tiles = 0;
  int start_x = rand() % (terrain_size - 1);
  int start_y = rand() % (terrain_size - 1);
  int distance_x = terrain_size - 1 - start_x;
  int distance_y = terrain_size - 1 - start_y;

  int min_distance;
  int lake_size;
  int max_size;
  int min_size;

  if(distance_x < distance_y) {
    max_size = distance_x;
  } else {
    max_size = distance_y;
  }
  min_size = 2;

  if(max_size <= min_size) {
    lake_size = max_size;
  } else {
    lake_size = (rand() % (max_size - min_size + 1)) + min_size;
  }

  for(int x = start_x; x <= start_x + lake_size; x++) {
    for(int y = start_y; y <= start_y + lake_size; y++) {
      terrain_grid[x][y].SetGround(WATER_TILE);
      no_tiles++;
    }
  }

  return no_tiles;
}

int Ecosystem::GenerateHills() {
  int no_tiles = 0;
  int min_size = 2;
  int max_size = 4;

  int hill_size = (rand() % (max_size - min_size + 1)) + min_size;

  int start_x;
  int start_y;
  bool stop;

  while(true) {
    stop = false;

    start_x = rand() % (terrain_size - hill_size);
    start_y = rand() % (terrain_size - hill_size);
    /* Check not to overlay a water tile */
    for(int x = start_x; x <= start_x + hill_size; x++) {
      for(int y = start_y; y <= start_y + hill_size; y++) {
        if(terrain_grid[x][y].GetGround() == WATER_TILE) {
          stop = true;
          break;
        }
      }
      if(stop == true) {
        break;
      }
    }
    if(stop == false) {
      break;
    }
  }

  /* Everything is ok now */
  for(int x = start_x; x <= start_x + hill_size; x++) {
    for(int y = start_y; y <= start_y + hill_size; y++) {
      terrain_grid[x][y].SetGround(HILL_TILE);
      no_tiles++;
    }
  }
  return no_tiles;
}

int Ecosystem::GenerateMeadow() {
  int no_tiles = 0;

  for(int x = 0; x < terrain_size; x++) {
    for(int y = 0; y < terrain_size; y++) {
      if(terrain_grid[x][y].GetGround() == EMPTY) {
        terrain_grid[x][y].SetGround(MEADOW_TILE);
        no_tiles++;
      }
    }
  }

  return no_tiles;
}

void Ecosystem::PrintGrid() {
  for(int i = 0; i < terrain_size; i++) {
    for(int j = 0; j < terrain_size; j++) {
      cout  << terrain_grid[i][j].GetGround();
    }
    cout << endl;
  }
}
