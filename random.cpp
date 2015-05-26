#include "random.h"

UniformRandomInt::UniformRandomInt(int a, int b)
	:min(a),delta(b-a)
{}

int UniformRandomInt::operator()()
{
	int r;
	int MAX_R = (RAND_MAX/(delta+1))*(delta+1)-1;
	while((r = rand()) > MAX_R);

	return min + r%(delta+1);
}

DiscreteRandomInt::DiscreteRandomInt(double* weights, int n)
	:total(0),n(n),weights(new double[n])
{
	for(int i = 0; i < n; ++i)
		this->weights[i] = weights[i], total+= weights[i];
}

DiscreteRandomInt::~DiscreteRandomInt()
{
	delete[] weights;
}

int DiscreteRandomInt::operator()()
{
	double val = double(rand())/RAND_MAX*total;

	double temp = 0;
	for(int i = 0; i < n; ++i)
		if(val < (temp += weights[i]))
			return i;
	
	return n-1;
}
