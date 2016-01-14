#include "testbase.hh"
#include "maze.hh"

#include <cassert>
#include <cstdlib>
#include <iostream>


using namespace std;


/*===========================================================================
 * Test code for two-argument constructor
 */

void test_constructor(TestContext &ctx) {
    ctx.DESC("Two-argument constructor");
    
    // Put these in separate blocks to force constructor/destructor calls.
    {
        Maze m(50, 30);
        ctx.CHECK(m.getNumRows() == 50);
        ctx.CHECK(m.getNumCols() == 30);
        
        // Write and then read each cell just so we make sure we can do that.
        // We will do more exhaustive tests later.
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                m.setCell(r, c, MazeCell::EMPTY);
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);
            }
        }
    }

    {
        Maze m(30, 50);
        ctx.CHECK(m.getNumRows() == 30);
        ctx.CHECK(m.getNumCols() == 50);
        
        // Write and then read each cell just so we make sure we can do that.
        // We will do more exhaustive tests later.
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                m.setCell(r, c, MazeCell::EMPTY);
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);
            }
        }
    }
    
    ctx.result();
}


/*===========================================================================
 * Test code for get/set start/end value
 */

void test_start_end(TestContext &ctx) {
    ctx.DESC("Get/set start/end");
    
    // Put these in separate blocks to force constructor/destructor calls.
    Maze m(10, 20);
    
    m.setStart(3, 5);
    m.setEnd(2, 1);
    
    ctx.CHECK(m.getStart() == Location(3, 5));
    ctx.CHECK(m.getEnd() == Location(2, 1));
    
    m.setStart(8, 18);
    m.setEnd(0, 6);
    
    ctx.CHECK(m.getStart() == Location(8, 18));
    ctx.CHECK(m.getEnd() == Location(0, 6));
    
    ctx.result();
}


/*===========================================================================
 * Test code for get-neighbor-cell operation
 */

void test_neighbor(TestContext &ctx) {
    ctx.DESC("Get neighbor");
    
    Maze m(6, 6);
    
    ctx.CHECK(m.getNeighborCell(3, 2, Direction::NORTH) == Location(2, 2));
    ctx.CHECK(m.getNeighborCell(3, 2, Direction::SOUTH) == Location(4, 2));
    ctx.CHECK(m.getNeighborCell(3, 2, Direction::EAST) == Location(3, 3));
    ctx.CHECK(m.getNeighborCell(3, 2, Direction::WEST) == Location(3, 1));
    
    ctx.result();
}


/*===========================================================================
 * Test code for get/set cell operations
 */

bool check_only_cell_visited(const Maze &m, int r_check, int c_check) {
    for (int r = 0; r < m.getNumRows(); r++) {
        for (int c = 0; c < m.getNumCols(); c++) {
            MazeCell expected = MazeCell::EMPTY;
            if (r == r_check && c == c_check)
                expected = MazeCell::VISITED;
            
            if (m.getCell(r, c) != expected)
                    return false;
        }
    }
    return true;
}


void test_get_set_cell(TestContext &ctx) {
    ctx.DESC("Get/set cell");
    
    Maze m(8, 16);
    m.clear();
    
    for (int r = 0; r < m.getNumRows(); r++) {
        for (int c = 0; c < m.getNumCols(); c++) {
            m.setCell(r, c, MazeCell::VISITED);
            ctx.CHECK(check_only_cell_visited(m, r, c));
            
            m.setCell(r, c, MazeCell::EMPTY);
            ctx.CHECK(check_only_cell_visited(m, -1, -1));
        }
    }
    
    ctx.result();
}


/*===========================================================================
 * Test code for has-wall/set-wall/clear-wall operations
 */

Direction opposite_direction(Direction dir) {
    switch (dir) {
    case Direction::NORTH:
        return Direction::SOUTH;

    case Direction::SOUTH:
        return Direction::NORTH;

    case Direction::EAST:
        return Direction::WEST;

    case Direction::WEST:
        return Direction::EAST;
    }
    assert(false);
}


bool cell_has_one_wall(const Maze &m, int r, int c, Direction dir) {
    return m.hasWall(r, c, dir) &&
           (!m.hasWall(r, c, Direction::NORTH) || dir == Direction::NORTH) &&
           (!m.hasWall(r, c, Direction::EAST ) || dir == Direction::EAST ) &&
           (!m.hasWall(r, c, Direction::SOUTH) || dir == Direction::SOUTH) &&
           (!m.hasWall(r, c, Direction::WEST ) || dir == Direction::WEST );
}


bool check_only_wall_set(const Maze &m,
                         int r_check, int c_check, Direction dir_check) {
    // The neighbor cell will also see the wall
    bool hasNeighbor =
        (r_check > 0 && dir_check == Direction::NORTH) ||
        (r_check < m.getNumRows() - 1 && dir_check == Direction::SOUTH) ||
        (c_check > 0 && dir_check == Direction::WEST) ||
        (c_check < m.getNumCols() - 1 && dir_check == Direction::EAST);

    Location neighbor(-1, -1);
    Direction opposite_dir = opposite_direction(dir_check);

    if (hasNeighbor)
        neighbor = m.getNeighborCell(r_check, c_check, dir_check);

    for (int r = 0; r < m.getNumRows(); r++) {
        for (int c = 0; c < m.getNumCols(); c++) {
            if (r == r_check && c == c_check) {
                // We expect that one of the adjacent walls should be set.
                if (!cell_has_one_wall(m, r, c, dir_check)) {
                    return false;
                }
            }
            else if (hasNeighbor && r == neighbor.row && c == neighbor.col) {
                // This cell borders the wall that was created.
                // We expect that one of the adjacent walls should be set.
                if (!cell_has_one_wall(m, r, c, opposite_dir)) {
                    return false;
                }
            }
            else if (m.hasWall(r, c, Direction::NORTH) ||
                     m.hasWall(r, c, Direction::EAST) ||
                     m.hasWall(r, c, Direction::SOUTH) ||
                     m.hasWall(r, c, Direction::WEST)) {
                return false;
            }
        }
    }
    return true;
}


bool check_no_wall_set(const Maze &m) {
    for (int r = 0; r < m.getNumRows(); r++) {
        for (int c = 0; c < m.getNumCols(); c++) {
            if (m.hasWall(r, c, Direction::NORTH) ||
                m.hasWall(r, c, Direction::EAST) ||
                m.hasWall(r, c, Direction::SOUTH) ||
                m.hasWall(r, c, Direction::WEST)) {
                return false;
            }
        }
    }
    return true;
}


void test_walls(TestContext &ctx) {
    ctx.DESC("Get/set wall");
     
    Maze m(8, 16);
    m.clear();
    
    for (int r = 0; r < m.getNumRows(); r++) {
        for (int c = 0; c < m.getNumCols(); c++) {
            m.setWall(r, c, Direction::NORTH);
            ctx.CHECK(check_only_wall_set(m, r, c, Direction::NORTH));
            
            m.clearWall(r, c, Direction::NORTH);
            ctx.CHECK(check_no_wall_set(m));

            m.setWall(r, c, Direction::EAST);
            ctx.CHECK(check_only_wall_set(m, r, c, Direction::EAST));
            
            m.clearWall(r, c, Direction::EAST);
            ctx.CHECK(check_no_wall_set(m));

            m.setWall(r, c, Direction::SOUTH);
            ctx.CHECK(check_only_wall_set(m, r, c, Direction::SOUTH));
            
            m.clearWall(r, c, Direction::SOUTH);
            ctx.CHECK(check_no_wall_set(m));

            m.setWall(r, c, Direction::WEST);
            ctx.CHECK(check_only_wall_set(m, r, c, Direction::WEST));
            
            m.clearWall(r, c, Direction::WEST);
            ctx.CHECK(check_no_wall_set(m));
        }
    }
    
    ctx.result();
}


/*===========================================================================
 * Test code for clear() operation
 */

void test_clear(TestContext &ctx) {
    ctx.DESC("clear() operation");
    
    // Put these in separate blocks to force constructor/destructor calls.
    {
        Maze m(50, 30);
        m.clear();
        
        // Verify that everything is cleared
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);
                
                // Yes, this will check many walls multiple times, but it's
                // the easiest most brain-dead way to test.
                ctx.CHECK(!m.hasWall(r, c, Direction::NORTH));
                ctx.CHECK(!m.hasWall(r, c, Direction::WEST));
                ctx.CHECK(!m.hasWall(r, c, Direction::SOUTH));
                ctx.CHECK(!m.hasWall(r, c, Direction::EAST));
            }
        }
    }

    {
        Maze m(30, 50);
        m.clear();
        
        // Verify that everything is cleared
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);

                ctx.CHECK(!m.hasWall(r, c, Direction::NORTH));
                ctx.CHECK(!m.hasWall(r, c, Direction::WEST));
                ctx.CHECK(!m.hasWall(r, c, Direction::SOUTH));
                ctx.CHECK(!m.hasWall(r, c, Direction::EAST));
            }
        }
    }
    
    ctx.result();
}


/*===========================================================================
 * Test code for set-all-walls operation
 */

void test_set_all_walls(TestContext &ctx) {
    ctx.DESC("setAllWalls() operation");
    
    // Put these in separate blocks to force constructor/destructor calls.
    {
        Maze m(50, 30);
        m.clear();
        m.setAllWalls();
        
        // Verify that all walls are set
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);
                
                // Yes, this will check many walls multiple times, but it's
                // the easiest most brain-dead way to test.
                ctx.CHECK(m.hasWall(r, c, Direction::NORTH));
                ctx.CHECK(m.hasWall(r, c, Direction::WEST));
                ctx.CHECK(m.hasWall(r, c, Direction::SOUTH));
                ctx.CHECK(m.hasWall(r, c, Direction::EAST));
            }
        }
    }

    {
        Maze m(30, 50);
        m.clear();
        m.setAllWalls();
        
        // Verify that all walls are set
        for (int r = 0; r < m.getNumRows(); r++) {
            for (int c = 0; c < m.getNumCols(); c++) {
                ctx.CHECK(m.getCell(r, c) == MazeCell::EMPTY);

                ctx.CHECK(m.hasWall(r, c, Direction::NORTH));
                ctx.CHECK(m.hasWall(r, c, Direction::WEST));
                ctx.CHECK(m.hasWall(r, c, Direction::SOUTH));
                ctx.CHECK(m.hasWall(r, c, Direction::EAST));
            }
        }
    }

    ctx.result();
}


/*===========================================================================
 * Test code for copy constructor
 *
 * We have this so far down the sequence because we want to be sure everything
 * else works before we try out copying things.
 */

void test_copy_ctor(TestContext &ctx) {
    ctx.DESC("Copy constructor");
    
    Maze m1(4, 6);
    m1.clear();
    
    m1.setCell(2, 2, MazeCell::VISITED);
    m1.setWall(1, 3, Direction::EAST);
    m1.setCell(3, 5, MazeCell::VISITED);
    m1.setWall(3, 4, Direction::SOUTH);
    
    ctx.CHECK(m1.getCell(2, 2) == MazeCell::VISITED);
    ctx.CHECK(m1.getCell(3, 5) == MazeCell::VISITED);
    ctx.CHECK(m1.hasWall(1, 3, Direction::EAST));
    ctx.CHECK(m1.hasWall(3, 4, Direction::SOUTH));
    
    Maze m2(m1);
    
    ctx.CHECK(m2.getCell(2, 2) == MazeCell::VISITED);
    ctx.CHECK(m2.getCell(3, 5) == MazeCell::VISITED);
    ctx.CHECK(m2.hasWall(1, 3, Direction::EAST));
    ctx.CHECK(m2.hasWall(3, 4, Direction::SOUTH));
    
    m1.setCell(2, 2, MazeCell::EMPTY);
    ctx.CHECK(m1.getCell(2, 2) == MazeCell::EMPTY);
    ctx.CHECK(m2.getCell(2, 2) == MazeCell::VISITED);
    
    m2.clearWall(3, 4, Direction::SOUTH);
    ctx.CHECK(m1.hasWall(3, 4, Direction::SOUTH));
    ctx.CHECK(!m2.hasWall(3, 4, Direction::SOUTH));
    
    ctx.result();
}


/*===========================================================================
 * Test code for assignment operator
 *
 * We have this so far down the sequence because we want to be sure everything
 * else works before we try out copying things.
 */

void test_assignment(TestContext &ctx) {
    ctx.DESC("Assignment operator");
    
    Maze m1(12, 8);
    m1.clear();
    m1.setStart(2, 3);
    m1.setEnd(9, 7);
    
    m1.setCell(2, 2, MazeCell::VISITED);
    m1.setWall(1, 3, Direction::EAST);
    m1.setCell(3, 5, MazeCell::VISITED);
    m1.setWall(3, 4, Direction::SOUTH);
    
    ctx.CHECK(m1.getCell(2, 2) == MazeCell::VISITED);
    ctx.CHECK(m1.getCell(3, 5) == MazeCell::VISITED);
    ctx.CHECK(m1.hasWall(1, 3, Direction::EAST));
    ctx.CHECK(m1.hasWall(3, 4, Direction::SOUTH));
    
    Maze m2(4, 4);
    m2.clear();
    m2.setStart(0, 0);
    m2.setEnd(1, 1);
    
    m2 = m1;
    ctx.CHECK(m2.getNumRows() == 12);
    ctx.CHECK(m2.getNumCols() == 8);
    ctx.CHECK(m2.getStart() == m1.getStart());
    ctx.CHECK(m2.getEnd() == m1.getEnd());
    
    ctx.CHECK(m2.getCell(2, 2) == MazeCell::VISITED);
    ctx.CHECK(m2.getCell(3, 5) == MazeCell::VISITED);
    ctx.CHECK(m2.hasWall(1, 3, Direction::EAST));
    ctx.CHECK(m2.hasWall(3, 4, Direction::SOUTH));
    
    m1.setCell(2, 2, MazeCell::EMPTY);
    ctx.CHECK(m1.getCell(2, 2) == MazeCell::EMPTY);
    ctx.CHECK(m2.getCell(2, 2) == MazeCell::VISITED);
    
    m2.clearWall(3, 4, Direction::SOUTH);
    ctx.CHECK(m1.hasWall(3, 4, Direction::SOUTH));
    ctx.CHECK(!m2.hasWall(3, 4, Direction::SOUTH));
    
    ctx.result();
    
    ctx.DESC("Self-assignment");
    
    // Why not?
    m1 = m1 = m1;
    
    ctx.CHECK(m1.getCell(2, 2) == MazeCell::EMPTY);
    ctx.CHECK(m1.getCell(3, 5) == MazeCell::VISITED);
    ctx.CHECK(m1.hasWall(1, 3, Direction::EAST));
    ctx.CHECK(m1.hasWall(3, 4, Direction::SOUTH));
    
    ctx.result();
}


/*===========================================================================
 * Main program to run tests!
 */

/*! This program is a simple test-suite for the Maze class. */
int main() {
  
    cout << "Testing the Maze class." << endl << endl;

    // srand(654321L);

    TestContext ctx(cout);

    test_constructor(ctx);
    test_start_end(ctx);
    test_neighbor(ctx);
    test_get_set_cell(ctx);
    test_walls(ctx);
    test_clear(ctx);
    test_set_all_walls(ctx);
    test_copy_ctor(ctx);
    test_assignment(ctx);
    
    // Return 0 if everything passed, nonzero if something failed.
    return !ctx.ok();
}
