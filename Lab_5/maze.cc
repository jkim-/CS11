#include "maze.hh"

// Helper functions
// Returns 1D array index of the 2D expanded coords
int Maze::getArrayIndex(const Location &loc) const {
  int oneDimIndex = (loc.row * getNumExpCols()) + loc.col; 
  return oneDimIndex;
}

// Returns the total number of elements in the expanded coords
int Maze::getNumExpCoords() const {
  int numEntries = (2 * getNumRows() + 1) * (2 * getNumCols() + 1);
  return numEntries;
}

// Returns the 2D expanded coordinates of the cell
Location Maze::getCellArrayCoord(int cellRow, int cellCol) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location expCellLoc = Location(2 * cellRow + 1, 2 * cellCol + 1);
  return expCellLoc;
}

// Returns the 2D expanded coordinates of the wall
Location Maze::getWallArrayCoord(int cellRow, int cellCol,
                                 Direction direction) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location expWallLoc = getCellArrayCoord(cellRow, cellCol);
  switch(direction) {
    case Direction::NORTH: expWallLoc.row -= 1; break; 
    case Direction::EAST: expWallLoc.col += 1; break; 
    case Direction::SOUTH: expWallLoc.row += 1; break; 
    case Direction::WEST: expWallLoc.col -= 1; break; 
  }
  return expWallLoc;
}

// Maze constructor
Maze::Maze(int rows, int cols) {
  numRows = rows;
  numCols = cols;
  int numEntries = getNumExpCoords();
  cells = new MazeCell[numEntries];
  for (int i = 0; i < numEntries; i++) cells[i] = MazeCell::EMPTY;
  start = Location(0, 0);
  end = Location(0, 0);
}

// Maze copy constructor
Maze::Maze(const Maze &m) {
  numRows = m.getNumRows();
  numCols = m.getNumCols();
  int numEntries = m.getNumExpCoords();
  cells = new MazeCell[numEntries];
  for (int i = 0; i < numEntries; i++) cells[i] = m.cells[i];
  start = m.getStart();
  end = m.getEnd();
}

// Maze destructor
Maze::~Maze() {
  delete[] cells; 
}

// Maze assignment operator
Maze& Maze::operator=(const Maze &m) {
  if (this != &m) {
    delete[] cells;
    numRows = m.getNumRows();
    numCols = m.getNumCols();
    int numEntries = m.getNumExpCoords();
    cells = new MazeCell[numEntries];
    for (int i = 0; i < numEntries; i++) cells[i] = m.cells[i];
    start = m.getStart();
    end = m.getEnd();
  }
  return *this;
}

// Member functions

// Sets all cells and walls to be empty, so that the maze is
// completely cleared
void Maze::clear() {
  int numEntries = getNumExpCoords();
  for (int i = 0; i < numEntries; i++) cells[i] = MazeCell::EMPTY;
}

// Iterate over all cells and set walls on each one's 4 sides
void Maze::setAllWalls() {
  for (int i = 0; i < getNumRows(); i++) {
    for (int j = 0; j < getNumCols(); j++) {
      for (int k = (int) Direction::NORTH; 
           static_cast<Direction>(k) <= Direction::WEST; k++) {
        Direction wallDir = static_cast<Direction>(k); 
        setWall(i, j, wallDir);
      }
    }
  }
}

// Returns the state of the specified cell
MazeCell Maze::getCell(int cellRow, int cellCol) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location cellLoc = getCellArrayCoord(cellRow, cellCol);
  int cellIndex = getArrayIndex(cellLoc);
  return cells[cellIndex];
}

// Set the state of the specified cell
void Maze::setCell(int cellRow, int cellCol, MazeCell val) {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location cellLoc = getCellArrayCoord(cellRow, cellCol);
  int cellIndex = getArrayIndex(cellLoc);
  cells[cellIndex] = val;
}

// Returns the cell-coordinates of the neighboring cell in the specified
// direction.  Trips an assertion if the given cell has no neighbor in the
// specified direction (e.g. the NORTH neighbor of cell (0,5)).
Location Maze::getNeighborCell(int cellRow, int cellCol,
                               Direction direction) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location neighborCellLoc;
  switch(direction) {
    case Direction::NORTH: 
      {
        assert(cellRow > 0);
        neighborCellLoc = Location(cellRow - 1, cellCol);
        break;
      }
    case Direction::EAST: 
      {
        assert(cellCol < numCols - 1);
        neighborCellLoc = Location(cellRow, cellCol + 1);
        break;
      }
    case Direction::SOUTH: 
      {
        assert(cellRow < numRows - 1);
        neighborCellLoc = Location(cellRow + 1, cellCol);
        break;
      }
    case Direction::WEST: 
      {
        assert(cellCol > 0);
        neighborCellLoc = Location(cellRow, cellCol - 1);
        break;
      }
  }
  return neighborCellLoc;
}

// Returns true if there is a wall in the specified direction from the
// given cell, false otherwise
bool Maze::hasWall(int cellRow, int cellCol, Direction direction) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location wallLoc = getWallArrayCoord(cellRow, cellCol, direction);
  int wallIndex = getArrayIndex(wallLoc);
  return cells[wallIndex] == MazeCell::WALL;
}

// Puts a wall on the specified side of the given cell
void Maze::setWall(int cellRow, int cellCol, Direction direction) {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location wallLoc = getWallArrayCoord(cellRow, cellCol, direction);
  int wallIndex = getArrayIndex(wallLoc);
  cells[wallIndex] = MazeCell::WALL;
}

// Removes a wall on the specified side of the given cell
void Maze::clearWall(int cellRow, int cellCol, Direction direction) {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location wallLoc = getWallArrayCoord(cellRow, cellCol, direction);
  int wallIndex = getArrayIndex(wallLoc);
  cells[wallIndex] = MazeCell::EMPTY;
}

// Returns true if the specified maze cell has been visited.
bool Maze::isVisited(int cellRow, int cellCol) const {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location cellLoc = getCellArrayCoord(cellRow, cellCol);
  int cellIndex = getArrayIndex(cellLoc);
  return cells[cellIndex] == MazeCell::VISITED;
}

// Changes the cell's value to VISITED
void Maze::setVisited(int cellRow, int cellCol) {
  // Assert viable coordinates
  assert((cellRow >= 0) && (cellRow < getNumRows()));
  assert((cellCol >= 0) && (cellCol < getNumCols()));

  Location cellLoc = getCellArrayCoord(cellRow, cellCol);
  int cellIndex = getArrayIndex(cellLoc);
  cells[cellIndex] = MazeCell::VISITED;
}

void Maze::print(std::ostream &os) const {
  os << getNumRows() << " " << getNumCols() << std::endl;
  int numExpRows = getNumExpRows();
  int numExpCols = getNumExpCols();
  Location start = getStart();
  Location startExp = getCellArrayCoord(start.row, start.col);
  Location end = getEnd();
  Location endExp = getCellArrayCoord(end.row, end.col);
  for (int r = 0; r < numExpRows; r++) {
    for (int c = 0; c < numExpCols; c++) {
      Location curLoc = Location(r, c);
      int curIndex = getArrayIndex(curLoc);
      if (r % 2 == 0 && c % 2 == 0) os << "+"; //  Neither cell or wall
      else if (r % 2 == 0 || c % 2 == 0) { //  Wall
        if (r % 2 == 0) { //  Horizontal walls
          if (cells[curIndex] == MazeCell::WALL) os << "---";
          else os << "   ";
        }
        else { //  Vertical walls
          if (cells[curIndex] == MazeCell::WALL) os << "|";
          else os << " ";
        }
      }
      else { //  Cell
        if (curLoc == startExp) os << " S ";
        else if (curLoc == endExp) os << " E ";
        else os << "   ";
      }
    }
    os << std::endl;
  }
}
