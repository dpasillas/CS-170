#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

//functor generates a random int in the range of [a,b]
// with uniform distribution
class UniformRandomInt{
public:
	UniformRandomInt(int a, int b);
	int operator()();

private:
	int min, delta;
};

//functor generates a random int in the range of [0,n-1]
// with a weighted distribution
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

