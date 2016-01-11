//Header file for TSPGenome class
#include <vector>
#include "Point.hh"

class TSPGenome {
	private:
		std::vector<int> _order;
		double _circuitLength;

	public:
		//Constructors
		TSPGenome(const int numPoints);
		TSPGenome(const std::vector<int> &order);
		
		//Destructor
		~TSPGenome();

		//Mutator methods
		
		//Accessor methods
		inline std::vector<int> getOrder() const {
			return _order;
		}

		inline double getCircuitLength() const {
			return _circuitLength;
		}
		
		//Member functions
		void computeCircuitLength(const std::vector<Point> &points);
		void mutate();
};

TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2);

bool isShorterPath(const TSPGenome &g1, const TSPGenome &g2);

TSPGenome findAShortPath(const std::vector<Point> &points,
		                     int populationSize, int numGenerations,
												 int keepPopulation, int numMutations);

void setRandInt(int &i, const int start, const int end);
void setTwoDiffRandInts(int &i, int &j, const int start, const int end);
