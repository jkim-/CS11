#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "tsp-ga.hh"

using namespace std;

void displayPath(const vector<int> &order);
void usage(const char *progname);

int main(int argc, char **argv) {

	//Variables to hold user input points
  if (argc != 5) {
    usage(argv[0]);
    return 1;
  }

  const int population = (int) atoi(argv[1]);
  const int generations = (int) atoi(argv[2]);
  const float keepFraction = (float) atof(argv[3]);
  const float mutationFactor = (float) atof(argv[4]);

  if (population < 1 || generations < 1 || keepFraction < 0 ||
      keepFraction > 1 || mutationFactor < 0) {
    usage(argv[0]);
    return 1;
  }

	int nPoints;
	double x, y, z;
	Point p;
	vector<Point> usrPoints;

	//Ask and record user inputs
 	cout << "This program approximately solves the TSP in 3D efficiently." << endl
 				<< "\nHow many points?";
 	cin >> nPoints;
 	cout << endl;
 	cout << "Enter the 3 coordinates of each point separated by space:" << endl;

	for (int i = 0; i < nPoints; i++) {
		cout << "Point " << i << ":";
		cin >> x >> y >> z;
		p.setX(x);
		p.setY(y);
		p.setZ(z);
		usrPoints.push_back(p);
		cout << endl;
	}

	//Find shortest path and output the result
	TSPGenome shortPath(nPoints);
	shortPath = findAShortPath(usrPoints, population, generations,
                            keepFraction * population,
                            mutationFactor * population);
	displayPath(shortPath.getOrder());

	//Display its length
	cout << "Shortest distance: " << shortPath.getCircuitLength() << endl; 

	return 0;
}

void displayPath(const vector<int> &order) {

	//Iterate over given vector and display each element
	cout << "Best order: [";
	for(unsigned int i = 0; i < order.size()-1; i++) {
		cout << order[i] << " ";
	}
	cout << order.back() << "]" << endl;
}

void usage(const char *progname) {
  cout << "Usage: " << progname << "population generations keep mutate"
       << endl;
  cout << "\npopulation: positive integer" << endl;
  cout << "generations: positive integer" << endl;
  cout << "keep: float between [0, 1]" << endl;
  cout << "mutate: nonnegative float" << endl;
}














