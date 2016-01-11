#include "tsp-ga.hh"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

using namespace std;

//Constructors
TSPGenome::TSPGenome(const int numPoints) {
	for (int i = 0; i < numPoints; i++) _order.push_back(i);
  random_device r;
  mt19937 g(r());
	shuffle(_order.begin(), _order.end(), g);
	_circuitLength = -1;
}

TSPGenome::TSPGenome(const vector<int> &order) {
	_order = order;
	_circuitLength = -1;
}

//Destructor
TSPGenome::~TSPGenome() {
}

//Mutator methods

//Accessor methods

//Member functions
void TSPGenome::computeCircuitLength(const vector<Point> &points) {

	//Iterate over _order and calculate circuit length
	double cumuLength = 0;
	vector<int>::iterator it = _order.begin();

	//Sum distance from first point to last point
	while (it != --_order.end()) {
		cumuLength += points[*it].distanceTo(points[*(it+1)]);
		it++;
	}

	//Add distance from last to first and assign circuitLength
	cumuLength += points[_order.front()].distanceTo(points[_order.back()]);
	_circuitLength = cumuLength;
}

void TSPGenome::mutate() {

	int swp1, swp2;
	setTwoDiffRandInts(swp1, swp2, 0, (int) _order.size()-1);

	//Mutate!
  swap(_order[swp1], _order[swp2]);
}

TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2) {

	vector<int> g1Order = g1.getOrder();
	vector<int> g2Order = g2.getOrder();
	int genomeLength = (int) g1Order.size();
	vector<int> orderOffspring;

	int cut;
	setRandInt(cut, 0, genomeLength - 1);
	set<int> s;

	for (int i = 0; i < cut; i++)	{
		orderOffspring.push_back(g1Order[i]);
		s.insert(g1Order[i]);
	}

	for (int i = 0; i < (int) g2Order.size(); i++) {
		if (!s.count(g2Order[i])) {
			orderOffspring.push_back(g2Order[i]);
			s.insert(g2Order[i]);
		}
		if ((int) orderOffspring.size() == genomeLength) break;
	}

	//Create TSPGnome offspring based on orderOffspring
	TSPGenome offspring(orderOffspring);

	return offspring;
}

bool isShorterPath(const TSPGenome &g1, const TSPGenome &g2) {
	if (g1.getCircuitLength() < g2.getCircuitLength()) return true;
	return false;
}

TSPGenome findAShortPath(const vector<Point> &points,
		                     int populationSize, int numGenerations,
												 int keepPopulation, int numMutations) {
	
	//Generate random population of genomes
	vector<TSPGenome> population;
	for (int i = 0; i < populationSize; i++) {
		TSPGenome g((int) points.size());
		g.computeCircuitLength(points);
		population.push_back(g);
	}

	for (int gen = 0; gen < numGenerations; gen++) { 

		//Sort by _circuitLength
		sort(population.begin(), population.end(), isShorterPath);
		
		//Print out progress
		if (gen % 10 == 0) {
			cout << "Generation " << gen << ": Shortest path is "
					 << population[0].getCircuitLength() << endl;
		}

		//Keep top keepPopulation individuals, re-generate the rest
		for (int i = keepPopulation; i < populationSize; i++) {
			int p1, p2;
			setTwoDiffRandInts(p1, p2, 0, keepPopulation - 1);
			population[i] = crosslink(population[p1], population[p2]);
      population[i].computeCircuitLength(points);
		}

		//Apply numMutations mutations (except on the best)
		for (int i = 0; i < numMutations; i++) {
			int m;
			setRandInt(m, 1, populationSize - 1);
			population[m].mutate();
		}
	}

  sort(population.begin(), population.end(), isShorterPath);
	return population[0];
}

void setRandInt(int &i, const int start, const int end) {
	random_device r;
	mt19937 g(r());
	uniform_int_distribution<int> rndInt(start, end);
	i = rndInt(g);
}


void setTwoDiffRandInts(int &i, int &j, const int start, const int end) {
	random_device r;
	mt19937 g(r());
	uniform_int_distribution<int> rndInt(start, end);
	i = 0, j = 0;
	while (i == j) {
		i = rndInt(g);
		j = rndInt(g);
	}
}
	













