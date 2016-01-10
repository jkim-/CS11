#include <iostream>
#include <vector>
#include <algorithm>
#include "Point.hh"

using namespace std;

double circuitLength(const vector<Point> &points, const vector<int> &order);
vector<int> findShortestPath(const vector<Point> &points);
void displayPath(const vector<int> &order);

int main() {

	//Variables to hold user input points
	int nPoints;
	double x, y, z;
	Point p;
	vector<Point> usrPoints;

	//Ask and record user inputs
 	cout << "This program solves the TSP in 3D inefficiently." << endl
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
	vector<int> bestPath;
	bestPath = findShortestPath(usrPoints);
	displayPath(bestPath);

	//Display its length
	cout << "Shortest distance: " << circuitLength(usrPoints, bestPath) << endl; 

	return 0;
}

double circuitLength(const vector<Point> &points, const vector<int> &order) {

	//Iterate over the order vector and calculate distance travelled
	double cumuLength = 0;
	vector<int>::const_iterator it = order.begin();

	//Sum distance from first point to last point
	while (it != --order.end()) {
		cumuLength += points[*it].distanceTo(points[*(it+1)]);
		it++;
	}

	//Add distance from last to first and exit
	cumuLength += points[order.front()].distanceTo(points[order.back()]);
	return cumuLength;
}

vector<int> findShortestPath(const vector<Point> &points) {
	
	//Path to be permuted and iterated over
	vector<int> curPath;
	for (unsigned int i = 0; i < points.size(); i++) curPath.push_back((int) i);

	//Holds best found path so far and its length
	double shortestLength = circuitLength(points, curPath);
	vector<int> bestPath = curPath;
	double curLength = 0;

	//Iterate over all possible paths and record the best path
	while (next_permutation(curPath.begin(),curPath.end())) {
		curLength = circuitLength(points, curPath);
		if (curLength < shortestLength) {
			shortestLength = curLength;
			bestPath = curPath;
		}
	}

	return bestPath;
}

void displayPath(const vector<int> &order) {

	//Iterate over given vector and display each element
	cout << "Best order: [";
	for(unsigned int i = 0; i < order.size()-1; i++) {
		cout << order[i] << " ";
	}
	cout << order.back() << "]" << endl;
}

















