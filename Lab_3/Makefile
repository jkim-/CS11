CXX = g++-4.9 -std=c++14 -Wall

tsp-ga: tsp-main.cc tsp-ga.cc Point.cc tsp-ga.hh Point.hh
	$(CXX) tsp-main.cc tsp-ga.cc Point.cc -o $@

.PHONY: clean
clean:
	\rm -f *.o *~ tsp-ga
