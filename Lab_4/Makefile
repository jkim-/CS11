CXX = g++-4.9 -std=c++14 -Wall

test-maze: maze.cc test-maze.cc testbase.cc maze.hh testbase.hh
	$(CXX) maze.cc test-maze.cc testbase.cc -o $@

.PHONY: clean
clean:
	\rm -f *.o *~ test-maze
