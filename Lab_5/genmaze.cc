#include "maze.hh"
#include <random>
#include <vector>
#include <iostream>

using namespace std;

void usage(const char *progname);
void getPossibleDirections(const Maze &maze, const Location &curLoc,
                           vector<Direction> &options);

int main(int argc, char **argv) {
  // Store user input variables and show usage if infeasible 
  if (argc != 3) {
    usage(argv[0]);
    return 1;
  }

  const int numRows = (int) atoi(argv[1]);
  const int numCols = (int) atoi(argv[2]);

  if (numRows < 0 || numCols < 0) {
    usage(argv[0]);
    return 1;
  }

  // Initialize the maze with all possible walls and
  // set start = (0, 0) and end = (numRows-1, numCols-1) 
  Maze maze(numRows, numCols);
  vector<Location> path;

  maze.clear();
  maze.setAllWalls();
  maze.setStart(0, 0);
  maze.setVisited(0, 0);
  maze.setEnd(numRows - 1, numCols - 1);

  // Generate a random maze
  path.push_back(maze.getStart());
  while (!path.empty()) {
    // Append current cell to the path
    Location curLoc = path.back();
    // If we reached the end, try another direction from previous cell
    if (curLoc == maze.getEnd()) {
      path.pop_back();
      continue;
    }
    // Get possible directions to go that hasn't been visited
    vector<Direction> options;
    getPossibleDirections(maze, curLoc, options);
    if (options.empty()) { //  Deadend
      path.pop_back();
      continue;
    }
    // Choose a random direction
    random_device r;
    mt19937 g(r());
    uniform_int_distribution<int> rndInt(0, options.size() - 1);
    int rndIndex = rndInt(g);
    Direction dir = options[rndIndex];
    // Clear the wall in that direction and move to that cell
    maze.clearWall(curLoc.row, curLoc.col, dir);
    Location nextCellLoc = maze.getNeighborCell(curLoc.row, curLoc.col, dir);
    maze.setVisited(nextCellLoc.row, nextCellLoc.col);
    path.push_back(nextCellLoc);
  }
  maze.print(cout);
  return 0;
}

void usage(const char *progname) {
  cout << "Usage: ./" << progname << " numRows numCols" << endl;
  cout << "\nnumRows is the number of rows in the maze" << endl;
  cout << "numCols is the number of columns in the maze" << endl;
}

void getPossibleDirections(const Maze &maze, const Location &curLoc,
                           vector<Direction> &options) {
  if (curLoc.row > 0 && !maze.isVisited(curLoc.row - 1, curLoc.col)) 
    options.push_back(Direction::NORTH);
  if (curLoc.col < maze.getNumCols() - 1 && !maze.isVisited(curLoc.row, curLoc.col + 1))
    options.push_back(Direction::EAST);
  if (curLoc.row < maze.getNumRows() - 1 && !maze.isVisited(curLoc.row + 1, curLoc.col))
    options.push_back(Direction::SOUTH);
  if (curLoc.col > 0 && !maze.isVisited(curLoc.row, curLoc.col - 1))
    options.push_back(Direction::WEST);
}
