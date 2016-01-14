#ifndef TESTBASE_HH
#define TESTBASE_HH


#include <iostream>
#include <set>
#include <string>
#include <cmath>

using namespace std;


class TestContext {                         // displays test results
    ostream &os;                            // output stream to use
    int passed;                             // # of tests which passed
    int total;                              // total # of tests
    int lastline;                           // line # of most recent test
    set<int> badlines;                      // line #'s of failed tests
    bool skip;                              // skip a line before title?

public:
    TestContext(ostream &os);               // write header to stream
    ~TestContext();                         // write summary info

    void desc(const string &msg, int line); // write line/description
    void check(bool test, int line);        // record if a check passes

    void result();                          // write test result
    bool ok() const;                        // true iff all tests passed
};


// ugly hacks
#define DESC(x) desc(x, __LINE__)
#define CHECK(test) check(test, __LINE__)

inline bool epsilon_equals(float a, float b, float epsilon = 0.00001) {
    return (fabsf(a - b) <= epsilon);
}

inline bool epsilon_equals(double a, double b, double epsilon = 0.00001) {
    return (fabs(a - b) <= epsilon);
}


#endif // TESTBASE_HH
