#include <iostream>
#include <cmath>
#include "Point.hh"

using namespace std;

double computeArea(Point &a, Point &b, Point &c);

int main() {

	Point p1, p2, p3;
	double x, y, z;

	cout << endl << "This program calculates the area of the triangle" << endl
			 <<	"formed by three points in 3D space." << endl << endl;

	cout << "Enter x, y, and z coordinates for the first point separated by space: ";
	cin >> x >> y >> z;	
	p1.setX(x);
	p1.setY(y);
	p1.setZ(z);
	cout << endl;

	cout << "Enter x, y, and z coordinates for the second point separated by space: ";
	cin >> x >> y >> z;	
	p2.setX(x);
	p2.setY(y);
	p2.setZ(z);
	cout << endl;

	cout << "Enter x, y, and z coordinates for the third point separated by space: ";
	cin >> x >> y >> z;	
	p3.setX(x);
	p3.setY(y);
	p3.setZ(z);
	cout << endl;

	cout << "Point 1: " << p1.getX() << ' ' << p1.getY() << ' ' << p1.getZ() << endl;
	cout << "Point 2: " << p2.getX() << ' ' << p2.getY() << ' ' << p2.getZ() << endl;
	cout << "Point 3: " << p3.getX() << ' ' << p3.getY() << ' ' << p3.getZ() << endl;
	cout << "Area is: " << computeArea(p1, p2, p3) << endl;
	
	return 0;
}

double computeArea(Point &a, Point &b, Point &c) {

	double s1 = a.distanceTo(b);
	double s2 = a.distanceTo(c);
	double s3 = b.distanceTo(c);
	double s = 0.5 * (s1 + s2 + s3);
	return sqrt(s * (s - s1) * (s - s2) * (s - s3));
}
