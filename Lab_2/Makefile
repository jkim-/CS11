CXX = g++-4.9 -std=c++14 -Wall

tsp: tsp.cc Point.cc Point.hh
	$(CXX) *.cc -o $@

.PHONY: clean
clean:
	\rm -f *.o *~ tsp
