CXX = g++-4.9
CXXFLAGS = -std=c++14 -Wall

all : genmaze

genmaze : genmaze.o maze.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean :
	rm -f genmaze *.o *~

.PHONY : all clean
