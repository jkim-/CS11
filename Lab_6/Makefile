CXX = g++-4.9
CXXFLAGS = -std=c++14 -Wall

all : test-regex

test-regex : test-regex.o engine.o regex.o testbase.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean :
	rm -f test-regex *.o *~

.PHONY : all clean
