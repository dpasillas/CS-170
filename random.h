#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

class UniformRandomInt{
public:
	UniformRandomInt(int a, int b);
	int operator()();

private:
	int min, delta;
};

class DiscreteRandomInt{
public:
	DiscreteRandomInt(double* weights, int n);
	~DiscreteRandomInt();
	int operator()();

private:
	double* weights;
	int n;
	double total;
};

#endif
