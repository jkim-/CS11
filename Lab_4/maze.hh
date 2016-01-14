#include <ostream>
#include <cassert>

// A simple class for representing locations in a 2D array.  The class also
// implements equality/inequality operators so that we can see if two
// locations are the same or not.
class Location {
public:
  // The row and column of the location
  int row;
  int col;

  // Constructors for initializing locations
  Location(int row, int col) : row(row), col(col) { }
  Location() : row(0), col(0) { }
  
  // Returns true if this location is the same as the specified location
  bool operator==(const Location &loc) const {
    return row == loc.row && col == loc.col;
  }
  
  // Returns true if this location is different from the specified location
  bool operator!=(const Location &loc) const {
    return !(*this == loc);
  }
};


// Possible values for maze cells
enum class MazeCell {
  EMPTY,
  WALL,
  VISITED
};


// Directions that we can go, relative to a given maze cell
enum class Direction {
  NORTH,
  EAST,
  SOUTH,
  WEST
};


class Maze {
private:
  // The number of rows with cells in them
  int numRows;
  
  // The number of columns with cells in them
  int numCols;
  
  // The maze's "expanded representation"
  MazeCell *cells;

  // The start of the maze, in cell coordinates
  Location start;
  
  // The end of the maze, in cell coordinates
  Location end;

  // Helper functions
  // Returns 1D array index of the 2D expanded coords
  int getArrayIndex(const Location &loc) const;
  
  // Returns the total number of elements in the expanded coords
  int getNumExpCoords() const;

  // Returns the number of rows in expanded coords
  inline int getNumExpRows() const {
    return 2 * numRows + 1;
  }
  
  // Returns the number of cols in expanded coords
  inline int getNumExpCols() const {
    return 2 * numCols + 1;
  }
  
  // Returns the 2D expanded coordinates of the cell
  Location getCellArrayCoord(int cellRow, int cellCol) const;
  
  // Returns the 2D expanded coordinates of the wall
  Location getWallArrayCoord(int cellRow, int cellCol,
                             Direction direction) const;

public:
  // Initialize a new maze of size rows x cols
  Maze(int rows, int cols);
  
  // Make a copy of an existing maze object
  Maze(const Maze &m);
  
  // Maze destructor
  ~Maze();
  
  // Maze assignment operator
  Maze& operator=(const Maze &m);


  // Returns the number of rows in the maze
  inline int getNumRows() const {
    return numRows;
  }
  
  // Returns the number of columns in the maze
  inline int getNumCols() const {
    return numCols;
  }

  // Returns the starting point in the maze
  inline Location getStart() const {
    return start;
  }

  // Sets the starting point in the maze    
  inline void setStart(int row, int col) {
    // Assert viable coordinates
    assert((row >= 0) && (row < numRows));
    assert((col >= 0) && (col < numCols));

    start = Location(row, col);
  }

  // Returns the ending point in the maze    
  inline Location getEnd() const {
    return end;
  }
  
  // Sets the ending point in the maze
  inline void setEnd(int row, int col) {
    // Assert viable coordinates
    assert((row >= 0) && (row < numRows));
    assert((col >= 0) && (col < numCols));

    end = Location(row, col);
  }

  // Sets all cells and walls to be empty, so that the maze is
  // completely cleared
  void clear();
  
  // Places a wall at every location that can be a wall in the maze
  void setAllWalls();


  // Returns the value of the specified
  MazeCell getCell(int cellRow, int cellCol) const;

  void setCell(int cellRow, int cellCol, MazeCell val);

  // Returns the cell-coordinates of the neighboring cell in the specified
  // direction.  Trips an assertion if the given cell has no neighbor in the
  // specified direction (e.g. the NORTH neighbor of cell (0,5)).
  Location getNeighborCell(int cellRow, int cellCol,
                           Direction direction) const;


  // Returns true if there is a wall in the specified direction from the
  // given cell, false otherwise
  bool hasWall(int cellRow, int cellCol, Direction direction) const;

  // Puts a wall on the specified side of the given cell
  void setWall(int cellRow, int cellCol, Direction direction);

  // Removes a wall on the specified side of the given cell
  void clearWall(int cellRow, int cellCol, Direction direction);

  // Returns true if the specified maze cell has been visited.
  bool isVisited(int cellRow, int cellCol) const;

  // Changes the cell's value to VISITED
  void setVisited(int cellRow, int cellCol);


  // Outputs the maze using simple ASCII-art to the specified output stream.
  // The output format is as follows, using the example maze from the
  // assignment write-up.  (The text to the right of the maze is purely
  // explanatory, and you don't need to output it.)
  //
  // 3 4                 (# of rows and then # of columns)
  // +---+---+---+---+   (each cell is 3 spaces wide, with a + at each corner)
  // | S     |       |   (walls indicated by --- or |)
  // +---+   +   +   +   (start indicated by S, end indicated by E)
  // |   |   |   |   |
  // +   +   +   +   +
  // |           | E |
  // +---+---+---+---+
  void print(std::ostream &os) const;
};
