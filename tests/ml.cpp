#include <cstdlib>
#include <iostream>
#include <random>

#include <activation.hpp>
#include <optimizer.hpp>

#include <std_activation_classes.hpp>
#include <std_optimizer_classes.hpp>

#include <matrix.hpp>
#include <vector.hpp>
#include <network.hpp>
#include <tensor.hpp>

using namespace std;
using namespace ml;

int main()
{
	srand(clock());

	DeepNeuralNetwork <double> model({
		{4, new Linear <double> ()},
		{10, new Linear <double> ()},
		{10, new Linear <double> ()},
		{1, new Linear <double> ()}
	}, []() {return 0.5 - rand()/(double) RAND_MAX;});

	cout << model({1, -1, 5, -2}) << endl;

	model.randomize();

	cout << model({1, 1, 1, 1}) << endl;

	model.randomize();

	cout << model({1, 1, 1, 1}) << endl;

	Tensor <int> tensor({3, 1}, 4);

	cout << tensor.print() << endl;

	Optimizer <double> opt;
	SquaredError <double> opt_s;
	MeanSquaredError <double> opt_ms;

	cout << "error: " << opt({3, 1, 4}, {4, 1, 5}) << endl;
	cout << "squared error: " << opt_s({3, 1, 4}, {4, 1, 5}) << endl;
	cout << "mean squared error: " << opt_ms({3, 1, 4}, {4, 1, 5}) << endl;
}