// C/C++ headers
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <complex>
#include <type_traits>

// Numerical headers
#include <gmpxx.h>

// Engine headers
#include <barn.h>
#include <complex.h>
#include <function.h>
#include <node.h>
#include <polynomial.h>
#include <rational.h>

using namespace std;

int main()
{
	string line;

	while (getline(cin, line)) {
		cout << "In: " << line << endl;

		node <double, int> nd = string(line);

		cout << "Out: " << nd.display() << "\n" << endl;
	}

	Function <double, int> f("f(x) = x^2");

	cout << f << endl;

	cout << f(10) << endl;
}